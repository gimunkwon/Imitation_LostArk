
#include "UI/HUD/LostArk_HUD.h"

#include "AssetViewUtils.h"
#include "Blueprint/UserWidget.h"
#include "Character/LostArk_Player.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"




void ALostArk_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerHUDClass)
	{
		PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
	
	
	if (BossHPWidgetClass)
	{
		// 위젯 생성 및 화면 부착
		BossHPWidget = CreateWidget<UUserWidget>(GetWorld(), BossHPWidgetClass);
		if (BossHPWidget)
		{
			BossHPWidget->AddToViewport();
			BossHPWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ALostArk_HUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 현재 퍼센트가 목표 퍼센트와 다르다면 부드럽게 보정
	if (!FMath::IsNearlyEqual(CurrentHPPercent, TargetHPPercent, 0.001f))
	{
		// FInterpTo : (현재값, 목표값, 프레임시간, 속도)
		CurrentHPPercent = FMath::FInterpTo(CurrentHPPercent, TargetHPPercent, DeltaSeconds, HPInterpSpeed);
		// 계산된 값을 실제 위젯의 ProgressBar에 적용
		if (BossHPWidget)
		{
			UProgressBar* HPBar = Cast<UProgressBar>(BossHPWidget->GetWidgetFromName(TEXT("BossHPBar")));
			if (HPBar)
			{
				HPBar->SetPercent(CurrentHPPercent);
			}
		}
	}
}

void ALostArk_HUD::UpdateBossHP(float CurrentHP, float MaxHP)
{
	// 목표치만 갱신
	if (MaxHP > 0)
	{
		TargetHPPercent = CurrentHP / MaxHP;
	}
}

void ALostArk_HUD::UpdateBossName(FString NewName)
{
	if (BossHPWidget)
	{
		UTextBlock* NameText = Cast<UTextBlock>(BossHPWidget->GetWidgetFromName(TEXT("Text_BossName")));
		if (NameText)
		{
			// FString을 UI용 타입인 FText로 변환하여 세팅
			NameText->SetText(FText::FromString(NewName));
		}
	}
}

void ALostArk_HUD::UpdateBossHPText(float CurrentHP, float MaxHP)
{
	if (BossHPWidget)
	{
		UTextBlock* HPText = Cast<UTextBlock>(BossHPWidget->GetWidgetFromName(TEXT("HPText")));
		if (HPText)
		{
			FString HPString = FString::Printf(TEXT("%.f | %.f"), CurrentHP, MaxHP);
			HPText->SetText(FText::FromString(HPString));
		}
	}
}

void ALostArk_HUD::UpdatePlayerHP(float CurrentHP, float MaxHP)
{
	if (PlayerHUDWidget)
	{
		UProgressBar* HPBar = Cast<UProgressBar>(PlayerHUDWidget->GetWidgetFromName(TEXT("HPBar")));
		UTextBlock* HPText = Cast<UTextBlock>(PlayerHUDWidget->GetWidgetFromName(TEXT("HPText")));
		
		if (HPBar) HPBar->SetPercent(CurrentHP / MaxHP);
		if (HPText) HPText->SetText(FText::FromString(FString::Printf(TEXT("%.f | %.f"), CurrentHP, MaxHP)));
	}
}

void ALostArk_HUD::UpdateDashCoolDown(float DashCoolTick)
{
	if (PlayerHUDWidget)
	{
		UTextBlock* DashCoolDownText = Cast<UTextBlock>(PlayerHUDWidget->GetWidgetFromName(TEXT("Text_DashCoolTime")));
		if (DashCoolDownText)
		{
			DashCoolDownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), DashCoolTick)));
		}
	}
}




