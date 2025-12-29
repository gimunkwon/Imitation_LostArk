
#include "Controller/Character_Controller.h"
#include "Character/LostArk_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


ACharacter_Controller::ACharacter_Controller()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ACharacter_Controller::BeginPlay()
{
	Super::BeginPlay();
	
	// 입력 컨텍스트 등록
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ACharacter_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// 클릭 시작과 끝 바인딩
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ACharacter_Controller::OnMoveStarted);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ACharacter_Controller::OnMoveReleased);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Canceled,this, &ACharacter_Controller::OnMoveReleased);
	}
}

void ACharacter_Controller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	// 마우스를 꾹 누르고 있는 동안 매 프레임 목적지 갱신
	if (bIsInputPressed)
	{
		MoveToMouseCursor();
	}
}

void ACharacter_Controller::OnMoveStarted()
{
	bIsInputPressed = true;
	if (ALostArk_Player* LostArk_Player = Cast<ALostArk_Player>(GetPawn()))
	{
		LostArk_Player->SetInputDirectionMode(true);
	}
}

void ACharacter_Controller::OnMoveReleased()
{
	bIsInputPressed = false;
	if (ALostArk_Player* LostArk_Player = Cast<ALostArk_Player>(GetPawn()))
	{
		LostArk_Player->SetInputDirectionMode(false);
	}
}

void ACharacter_Controller::MoveToMouseCursor()
{
	FHitResult Hit;
	// 마우스 아래 가시적인 지형 확인
	bool bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	
	if (bHitSuccessful)
	{
		// 내비게이션 메시를 이용한 이동
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
		// 클릭 이펙트
		if (ClickEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				ClickEffect,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation());
		}
	}
}
