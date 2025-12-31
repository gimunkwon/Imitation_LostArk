
#include "Enemy/LostArk_Enemy.h"

#include "AssetViewUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/LostArk_DamageText.h"
#include "UI/HUD/LostArk_HUD.h"


ALostArk_Enemy::ALostArk_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 감지 범위
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(DetectionRange);
	
	// 공격 범위 설정
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	AttackCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 이벤트 바인딩
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALostArk_Enemy::OnDetectionBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ALostArk_Enemy::OnDetectionEndOverlap);
}

void ALostArk_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALostArk_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALostArk_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ALostArk_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bIsDead) return ActualDamage; // 이미 죽었다면 무시
	
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.f, MaxHP);
	
	// 플레이어 컨트롤러를 통해 HUD 가져오기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD)
		{
			// HUD의 보스 체력바 업데이트
			HUD->UpdateBossHP(CurrentHP, MaxHP);
			HUD->UpdateBossHPText(CurrentHP, MaxHP);
		}
	}
	
	
	if (CurrentHP <= 0.f)
	{
		Die();
	}
	
	// 데미지 숫자를 띄울 위치
	if (DamageTextClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(50.f, 50.f, 80.f);
	
		// 약간의 랜덤성을 주기
		 SpawnLocation.X += FMath::RandRange(-20.f, 20.f);
		 SpawnLocation.Y += FMath::RandRange(-20.f, 20.f);
	
		// DamageText 액터를 스폰
		ALostArk_DamageText* DamageActor = GetWorld()->SpawnActor<ALostArk_DamageText>
		(DamageTextClass, SpawnLocation, FRotator::ZeroRotator);
		if (DamageActor)
		{
			DamageActor->SetDamageValue(ActualDamage);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit Reaming Health : %f"), CurrentHP);
	
	return ActualDamage;
}

void ALostArk_Enemy::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	// 충돌 판정 제거
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 3. AI 및 움직임 중지
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	
	if (DieEffect)
	{
		// 캐릭터의 현재 위치와 회전값으로 이펙트 생성
		UNiagaraComponent* DashFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DieEffect,
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
			},1.f, false);
		}
	}
	
	SetLifeSpan(1.f);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
		if (HUD && HUD->BossHPWidget)
		{
			HUD->BossHPWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy has been defeated!"));
}

void ALostArk_Enemy::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// 들어온 대상이 플레이어 인지 확인
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()) && OtherActor != this)
	{
		
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
			if (HUD && HUD->BossHPWidget)
			{
				// 이름 먼저 세팅
				HUD->UpdateBossName(EnemeyDisplayName);
				
				// HUD 표시 및 데이터 동기화
				HUD->BossHPWidget->SetVisibility(ESlateVisibility::Visible);
				HUD->UpdateBossHP(CurrentHP, MaxHP);
				HUD->UpdateBossHPText(CurrentHP, MaxHP);
			}
		}
		if (bIsAttacking)
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this
			, &ALostArk_Enemy::ExecuteAttack,AttackDelay, true);
		}
		
	}
}

void ALostArk_Enemy::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어가 범위를 벗어나면 HUD 숨기기
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD());
			if (HUD && HUD->BossHPWidget)
			{
				HUD->BossHPWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ALostArk_Enemy::ExecuteAttack()
{
	if (bIsDead) return;
	// 함수 실행 확인용 로그
	UE_LOG(LogTemp, Warning, TEXT("ExecuteAttack Timer Ticked"));
	// 공격 순간에만 충돌과 가시성 켜기
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCollision->SetHiddenInGame(false);
	
	
	// 공격 범위 내의 액터들 가져오기
	TArray<AActor*> OverlappingActors;
	AttackCollision->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			// 플레이어에게 데미지 전달
			UGameplayStatics::ApplyDamage(Actor, AttackDamage
				, GetController(),this, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Boss Attacked Player!"));
		}
	}
	// 짧은 시간 뒤에 박스를 다시 숨기고 충돌 끄기
	FTimerHandle ResetHandle;
	GetWorldTimerManager().SetTimer(ResetHandle, [this]()
	{
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollision->SetHiddenInGame(true);
	},0.2f, false);
}




