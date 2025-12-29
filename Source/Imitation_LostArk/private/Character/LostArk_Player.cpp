#include "Character/LostArk_Player.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
}

void ALostArk_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALostArk_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SmoothRotateToCursor(DeltaTime);
}

void ALostArk_Player::SmoothRotateToCursor(float DeltaTime)
{
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





