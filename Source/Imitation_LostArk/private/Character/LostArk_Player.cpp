#include "Character/LostArk_Player.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/LostArk_Enemy.h"
#include "Esther/Esther_Silian.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HUD/LostArk_HUD.h"


ALostArk_Player::ALostArk_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 기본 이동 방향 회전 옵션은 끄기
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
#pragma region CharacterRotate
	// 캐릭터가 이동 방향으로 자동으로 회전하게 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
#pragma endregion
	
#pragma region Camera&SpringArmComponent
	// 스프링 암 설정(카메라 지지대)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.f; // 카메라와 캐릭터 거리
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // 60도 각도로 아래로
	CameraBoom->bDoCollisionTest = false; // 장애물에 부딪혀도 카메라가 당겨지지 않게
	CameraBoom->bInheritPitch = false;	// 컨트롤러 회전 영향 안받게 고정
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	// 카메라 랙 설정
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 3.f;
	// 카메라 부착
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;
#pragma endregion
	// 무기 컴포넌트 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	// 무기를 캐릭터 메쉬의 소켓에 부착
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCanEverAffectNavigation(false);
}

void ALostArk_Player::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->UpdatePlayerHP(CurrentHP, MaxHP);
			HUD->UpdateDashCoolDown(DashCoolDown);
		}
	}
	
}

void ALostArk_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SmoothRotateToCursor(DeltaTime);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (bCanDash == false)
	{
		if (PC)
		{
			ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
			if (HUD)
			{
				// TimerHandle을 통해 남은 시간을 가져옴
				float RemainingTime = GetWorldTimerManager().GetTimerRemaining(DashTimerHandle);
				// 만약 남은 시간이 0보다 작으면 0으로 고정
				HUD->UpdateDashCoolDown(RemainingTime);
			}
		}
	}
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD)
		{
			// 스킬 쿨타임
			for (auto& Elem : SkillCooldownTimers)
			{
				FName SkillName = Elem.Key;
					
				float Remaining = GetWorldTimerManager().GetTimerRemaining(Elem.Value);
				// 0 보다 클 때만 업데이트
				if (Remaining > 0.f)
				{
					HUD->UpdateCooldownText(SkillName, Remaining);
				}
			}
			// 에스더 게이지
			HUD->UpdateEstherGauge(EstherGauge, MaxEstherGauge);
		}
	}
}

float ALostArk_Player::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.f, MaxHP);
	UE_LOG(LogTemp, Error, TEXT("Player HP: %.f / %.f"), CurrentHP, MaxHP);
	
	// 플레이어 HUD 업데이트
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD) HUD->UpdatePlayerHP(CurrentHP, MaxHP);
	}
	
	if (CurrentHP <= 0.f)
	{
		// 플레이어 사망 로직
		Die();
		UE_LOG(LogTemp, Error, TEXT("Player Dead!"));
	}
	
	return ActualDamage;
}
#pragma region Rotate
void ALostArk_Player::SmoothRotateToCursor(float DeltaTime)
{
	
	if (bIsAttacking || bIsDead)
	{
		return;
	}
	
	// 상태 체크 : 이동중이거나 공격 중일때만 아래 로직실행
	if (GetVelocity().Size() <= 0.1f)
	{
		return;
	}
	
	FRotator TargetRotation;
	
	// 마우스를 누르고 있는 상태 (드래그 이동 중)
	if (bIsInputHold)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility,false, TraceHitResult);
			if (TraceHitResult.bBlockingHit)
			{
				TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TraceHitResult.ImpactPoint);
			}
		}
	}
	// 클릭 후 자동으로 목적지 까지 이동중인 상태
	else
	{
		// 현재 이동하고 있는 방향을 바라보게 함
		TargetRotation = GetVelocity().ToOrientationRotator();
	}
	
	// Pitch와 Roll 고정 및 부드러운 보간
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;
	
	FRotator SmoothedRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	SetActorRotation(SmoothedRotation);
}
void ALostArk_Player::RotateToCursor()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility,false, Hit );
		if (Hit.bBlockingHit)
		{
			FVector TargetLocation = Hit.ImpactPoint;
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
			
			LookAtRotation.Pitch = 0.f;
			LookAtRotation.Roll = 0.f;
			
			SetActorRotation(LookAtRotation);
		}
	}
}
#pragma endregion 

#pragma region AttackFunc
void ALostArk_Player::Attack()
{
	if (bIsDead) return;
	// 이미 공격 중이면 중복 실행 방지
	if (bIsAttacking)
	{
		// 공격 중에 클릭하면 '다음 콤보 저장'
		bSaveCombo = true;
		return;
	}
	// 첫 공격 시작
	bIsAttacking = true;
	CurrentSkillData = nullptr;
	CurrentCombo = 1;
	bSaveCombo = false; // 초기화
	
	// 이동 중단
	if (GetController())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
	// 공격 방향으로 즉시 회전
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility,false, Hit);
		if (Hit.bBlockingHit)
		{
			FVector TargetLocation = Hit.ImpactPoint;
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
			LookAtRotation.Pitch = 0.f;
			LookAtRotation.Roll = 0.f;
			SetActorRotation(LookAtRotation);
		}
	}
	// 공격 상태 설정 및 몽타주 재생
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	
}

void ALostArk_Player::ComboCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("ComboCheck Notify Fired!"));
	
	if (bSaveCombo)
	{
		bSaveCombo = false;
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, 3);
		
		// 다음 공격 시작 전 마우스 방향으로 캐릭터 회전
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FHitResult Hit;
			PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
			if (Hit.bBlockingHit)
			{
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.ImpactPoint);
				LookAtRotation.Pitch = 0.f;
				LookAtRotation.Roll = 0.f;
				
				// 즉시 회전
				SetActorRotation(LookAtRotation);
			}
		}
		
		
		// 현재 콤보수에 맞는 섹션 이름을 생성
		FName NextSection = FName(*FString::Printf(TEXT("Attack%d"),CurrentCombo));
		
		UE_LOG(LogTemp, Warning, TEXT("Moving to Next Section: %s"), *NextSection.ToString());
		
		// 현재 재생 중인 몽타주의 섹션 강제 변경
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 현재 섹션이 어디인지 로그 출력
			FName CurrentSection = AnimInstance->Montage_GetCurrentSection(AttackMontage);
			UE_LOG(LogTemp, Warning, TEXT("Current : %s -> Next: %s")
				, *CurrentSection.ToString(), *NextSection.ToString());
			
			AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);
		}
	}
}

void ALostArk_Player::AttackHitCheck()
{
	// 1. 감지할 구체의 설정
	float AttackRange = 150.f; // 앞으로 뻗는거리
	float AttackRaduis = 150.f; // 구체의 크기(범위)
	
	FHitResult HitResult;
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.f; // 약간 앞에서 시작
	FVector End = Start + GetActorForwardVector() * AttackRange;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	// 2. 어떤 물체를 감지할 것인가?(Pawn 타입 감지)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	// 3. 스피어 트레이스 실행
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), Start, End, AttackRaduis , ObjectTypes, false, ActorsToIgnore
		, EDrawDebugTrace::ForDuration, HitResult, true);
	// 4. 충돌 시 대미지 전달
	if (bHit && HitResult.GetActor())
	{
		float DamageToApply = 20.f; // 기본 일반 공격 데미지
		// 만약 현재 스킬 데이터가 존재한다면
		if (CurrentSkillData)
		{
			DamageToApply = CurrentSkillData->Damage;
			// 2. 카운터 판정 시작
			// 현재 사용 중인 스킬이 카운터 스킬인가?
			if (CurrentSkillData->bIsCounterSkill)
			{
				ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(HitResult.GetActor());
				// 적이 보스 이고 현재 카운터가 가능한 상태인가?
				if (Enemy && Enemy->GetIsCounterable())
				{
					// 카운터 성공
					Enemy->SetEnemyState(EEnemyState::Groggy);
					Enemy->OnCounterSucces();
					UE_LOG(LogTemp, Error, TEXT("Counter Success"));
				}
			}
		}
		// 아까만든 Enemy 에게 대미지 보내기
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), DamageToApply
			, GetController(), this, nullptr);
		
		UE_LOG(LogTemp, Warning, TEXT("Hit Success : %s"), *HitResult.GetActor()->GetName());
		AddEstherGauge(10.f);
	}
}

void ALostArk_Player::EndCombo()
{
}


void ALostArk_Player::EndAttack()
{
	bIsAttacking = false;
	bSaveCombo = false;
	CurrentCombo = 0;
	CurrentSkillData = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Attack Ended"));
}
#pragma endregion

#pragma region DashFunc

void ALostArk_Player::Dash()
{
	if (!bCanDash || bIsDead) return;
	
	// 공격 중이라면 공격을 취소하고 대시로 전환
	 if (bIsAttacking)
	 {
		 // 재생 중인 공격 몽타주를 즉시 멈춤
	 	StopAnimMontage(AttackMontage);
	 	EndAttack();
	 	UE_LOG(LogTemp, Warning, TEXT("Attack Cancelled by Dash!"));
	 }
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility,false, Hit);
		
		if (Hit.bBlockingHit)
		{
			// 마우스 방향 계산
			FVector DashDirection = (Hit.ImpactPoint - GetActorLocation()).GetSafeNormal();
			DashDirection.Z = 0.f;
			
			if (DashSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			}
			// 대시 이펙트
			if (DashStartEffects)
			{
				// 캐릭터의 현재 위치와 회전값으로 이펙트 생성
				UNiagaraComponent* DashFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					DashStartEffects,
					GetActorLocation(),
					GetActorRotation());// 캐릭터가 보는 방향으로 이펙트 회전
				if (DashFX)
				{
					// 0.5초 뒤에 이펙트를 자동으로 끄도록 설정
					DashFX->SetAutoDestroy(true);
					
					FTimerHandle FXTimer;
					GetWorldTimerManager().SetTimer(FXTimer, [DashFX]()
					{
						if (DashFX) DashFX->Deactivate();
					},0.5f, false);
				}
			}
			
			// 마찰력을 무시하기 위해 무브먼트 모드를 잠시 조정하거나 속도를 강제 리셋
			GetCharacterMovement()->StopMovementImmediately();
			// 방향 전환
			SetActorRotation(DashDirection.ToOrientationRotator());
			
			DrawDebugLine(GetWorld(), GetActorLocation()
				, GetActorLocation() + DashDirection * 500.f, FColor::Red, false, 2.f, 0, 5.f);
			// 캐릭터 발사
			LaunchCharacter(DashDirection * DashImpulse, true, true);
			// 쿨타임 시작
			bCanDash = false;
			GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ALostArk_Player::ResetDash, DashCoolDown, false);
			
		}
	}
}

void ALostArk_Player::ResetDash()
{
	bCanDash = true;
	UE_LOG(LogTemp, Warning, TEXT("Dash is Ready!"));
}

#pragma endregion

void ALostArk_Player::UseSkill(FName SkillRowName)
{
	
	UE_LOG(LogTemp ,Warning, TEXT("Use Skill Func ON"));
	if (bIsAttacking || !SkillDataTable || bIsDead) return;
	
	// 쿨타임 체크
	if (SkillCooldownTimers.Contains(SkillRowName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill %s is on Cooldown!"), *SkillRowName.ToString());
		return;
	}
	
	
	// 데이터 테이블에서 데이터 찾기
	CurrentSkillData = SkillDataTable->FindRow<FSkillData>(SkillRowName, TEXT(""));
	
	if (CurrentSkillData && CurrentSkillData->SkillMontage)
	{
		bIsAttacking = true;
		
		// 이동 중단 및 회전
		GetCharacterMovement()->StopMovementImmediately();
		RotateToCursor();
		
		// 몽타주 재생
		PlayAnimMontage(CurrentSkillData->SkillMontage);
		
		// 쿨타임 타이머 시작
		float CooldownTime = CurrentSkillData->CoolDown;
		if (CooldownTime > 0.f)
		{
			FTimerHandle NewTimerHandle;
			
			GetWorldTimerManager().SetTimer(NewTimerHandle, [this, SkillRowName]()
			{
				SkillCooldownTimers.Remove(SkillRowName); // 쿨타임 맵에서 제거
				UE_LOG(LogTemp, Warning, TEXT("Skill %s Ready!"), *SkillRowName.ToString())
			}, CooldownTime, false);
			
			// 맵에 저장하여 관리
			SkillCooldownTimers.Add(SkillRowName, NewTimerHandle);
		}
	}
}


#pragma region Die&&Revive
void ALostArk_Player::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	// 모든 입력 및 이동 차단
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// 충돌 판정 제거
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 사망 애니메이션 재생
	if (DeathMontage)
	{
		float Duration = PlayAnimMontage(DeathMontage);
		
		PlayAnimMontage(DeathMontage);
		// 애니메이션이 끝날 때쯤 메쉬의 애니메이션을 일시정지
		
		GetWorldTimerManager().SetTimer(DeathAnimTimer, [this]()
		{
			if (GetMesh())
			{
				GetMesh()->bPauseAnims = true; // 애니메이션 일시정지
			}
		},Duration - 0.2f, false);
	}
	// 흑백 효과 적용
	ApplyGrayscaleEffect();
	// 2초 뒤에 ShowRestartUI 함수를 호출
	FTimerHandle RestartUITimer;
	GetWorldTimerManager().SetTimer(RestartUITimer, this, &ALostArk_Player::ShowRestartUI,4.f,false);
}

void ALostArk_Player::ApplyGrayscaleEffect()
{
	if (TopDownCamera)
	{
		// 포스트 프로세스 설정 가져오기
		FPostProcessSettings& Settings= TopDownCamera->PostProcessSettings;
		
		// Saturation(채도)을 0으로 만들어 흑백 처리
		Settings.bOverride_ColorSaturation = true;
		// X,Y,Z 가 각각 R, G, B 채도
		Settings.ColorSaturation = FVector4(0.f,0.f,0.f,1.f);
		// 화면을 약간 어둡게 하고 싶다면 가모나 노출을 조정
		Settings.bOverride_SceneFringeIntensity = true;
		Settings.SceneFringeIntensity = 5.f; // 화면 가장자리 왜곡 효과(필터 느낌)
	}
}

void ALostArk_Player::RemoveGrayscaleEffect()
{
	if (TopDownCamera)
	{
		FPostProcessSettings& Settings = TopDownCamera->PostProcessSettings;
		// 채도를 다시 1(정상)으로 되돌림
		Settings.bOverride_ColorSaturation = false;
		Settings.ColorSaturation = FVector4(1.f,1.f,1.f,1.f);
		Settings.bOverride_SceneFringeIntensity = false;
		Settings.SceneFringeIntensity = 0.f;
	}
}

void ALostArk_Player::ShowRestartUI()
{
	if (RestartWidgetClass)
	{
		RestartWidgetInstance = CreateWidget<UUserWidget>(GetWorld(),  RestartWidgetClass);
		if (RestartWidgetInstance)
		{
			RestartWidgetInstance->AddToViewport();
		}
	}
}

void ALostArk_Player::RevivePlayer()
{
	UE_LOG(LogTemp, Warning,TEXT("RevivePlayer On"));
	if (!bIsDead) return;
	
	// 혹시 아직 실행 대기중인 "사망 시 애니메이션 멈춤" 타이머가 있다면 취소
	GetWorldTimerManager().ClearTimer(DeathAnimTimer);
	// 상태및 체력 초기화
	bIsDead = false;
	CurrentHP = MaxHP;
	bSaveCombo = false;
	bIsAttacking = false;
	
	// 애니메이션 및 메쉬 초기화
	if (GetMesh())
	{
		GetMesh()->bPauseAnims = false; // 멈췄던 애니메이션 재개
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f); // 사망 몽타주 강제종료
	}
	
	// 충돌 및 이동복구
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->SetComponentTickEnabled(true);
	}
	
	// 입력 복구
	APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
	}
	// 시각 효과 복구
	RemoveGrayscaleEffect();
	// UI 제거
	if (RestartWidgetInstance != nullptr)
	{
		RestartWidgetInstance->RemoveFromParent();
		RestartWidgetInstance = nullptr;
	}
	// HUD 업데이트
	ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
	if (HUD)
	{
		UE_LOG(LogTemp, Error, TEXT("현재체력 : %f, 맥스체력 : %f"),CurrentHP, MaxHP);
		HUD->UpdatePlayerHP(CurrentHP, MaxHP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUD is Null"));
	}
}

#pragma endregion 

void ALostArk_Player::UseEstherSilian()
{
	if (EstherGauge < 100.f || bIsAttacking || bIsDead) return;
	
	
	// 소환 위치 계산
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.f;
	FRotator SpawnRotation = GetActorRotation();
	// 실리안 액터 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->StartEstherPresentation();
		}
	}
	
	AEsther_Silian* Silian = GetWorld()->SpawnActor<AEsther_Silian>(SilianClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Silian)
	{
		// 게이지 소모
		EstherGauge = 0.f;
	}

}



