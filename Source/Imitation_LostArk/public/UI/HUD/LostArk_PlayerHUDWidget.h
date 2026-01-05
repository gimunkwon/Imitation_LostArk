
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LostArk_PlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class IMITATION_LOSTARK_API ULostArk_PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SkillText_Q;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SkillText_W;
	// 이름으로 텍스트 박스를 찾기위한 맵
	TMap<FName, UTextBlock*> SkillTextMap;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* EstherProgressBar;
	
	
public:
	virtual void NativeConstruct() override;
	// 실제 텍스트를 업데이트하는 함수
	void UpdateCooldownText(FName SkillName, float RemainingTime);
	// 게이지 비율(0.0 ~ 1.0)을 받아 업데이트 하는 함수
	void UpdateEstherGauge(float CurrentRation);
	
};
