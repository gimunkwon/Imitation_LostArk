#include "UI/LostArk_DamageText.h"

#include "Components/WidgetComponent.h"


ALostArk_DamageText::ALostArk_DamageText()
{
	PrimaryActorTick.bCanEverTick = false;
	
	DamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	RootComponent = DamageWidget;
	
	// UI 가 화면을 바라보게 설정 (Screen Space)
	DamageWidget->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidget->SetDrawSize(FVector2D(200.f, 50.f));
}


void ALostArk_DamageText::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeTime);
}

void ALostArk_DamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



