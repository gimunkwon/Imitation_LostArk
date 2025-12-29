#include "Character/LostArk_Player.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ALostArk_Player::ALostArk_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 캐릭터가 이동 방향으로 자동으로 회전하게 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
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
}


void ALostArk_Player::BeginPlay()
{
	Super::BeginPlay();
	
}


void ALostArk_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALostArk_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

