// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LostArk_PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULostArk_PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SkillText_Q)
	{
		SkillTextMap.Add(TEXT("EnhancedAttack"), SkillText_Q);
	}
	if (SkillText_W)
	{
		SkillTextMap.Add(TEXT("CounterAttack"), SkillText_W);
	}
}

void ULostArk_PlayerHUDWidget::UpdateCooldownText(FName SkillName, float RemainingTime)
{
	// 맵에서 이름에 맞는 텍스트 위젯을 찾음
	if (SkillTextMap.Contains(SkillName))
	{
		UTextBlock* TargetText = SkillTextMap[SkillName];
		if (!TargetText) return;
		if (RemainingTime > 0.1f)
		{
			// 텍스트 보이기
			TargetText->SetVisibility(ESlateVisibility::Visible);
			FString TimeStr = FString::Printf(TEXT("%.1f"), RemainingTime);
			TargetText->SetText(FText::FromString(TimeStr));
		}
		else
		{
			// 0초면 숨기기
			TargetText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ULostArk_PlayerHUDWidget::UpdateEstherGauge(float CurrentRation)
{
	EstherProgressBar->SetPercent(CurrentRation);
	
	
	// 게이지가 다 찼을때 반짝이는 효과
	FLinearColor GaugeColor = (CurrentRation >= 1.f) ? FLinearColor::Yellow : FLinearColor::White;
	EstherProgressBar->SetFillColorAndOpacity(GaugeColor);
}
