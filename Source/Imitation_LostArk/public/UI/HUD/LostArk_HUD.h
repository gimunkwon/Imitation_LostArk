

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LostArk_HUD.generated.h"


class ULostArk_PlayerHUDWidget;
class UTextBlock;

UCLASS()
class IMITATION_LOSTARK_API ALostArk_HUD : public AHUD
{
	GENERATED_BODY()
public:
	// 위젯 클래스
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> BossHPWidgetClass;
	// 실제 생성된 위젯 인스턴스
	UUserWidget* BossHPWidget;
	// 체력 업데이트 함수
	void UpdateBossHP(float CurrentHP, float MaxHP);
	// 보스 이름을 위젯에 전달하는 함수
	void UpdateBossName(FString NewName);
	// 체력 숫자 텍스트를 업데이트하는 함수
	void UpdateBossHPText(float CurrentHP, float MaxHP);
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;
	UUserWidget* PlayerHUDWidget;
	void UpdatePlayerHP(float CurrentHP, float MaxHP);
	void UpdateDashCoolDown(float DashCoolTick);
	void UpdateCountText(FString BossName);
	void UpdateCooldownText(FName SkillName, float RemainingTime);
	void UpdateEstherGauge(float CurrentGauge, float MaxGauge);
	
	// 컷신 시작시 호출
	void StartEstherPresentation();
	void ClearCutSceneWidget() {CutSceneWidget = nullptr;}
	
	void ShowGameEndHUD();
protected:
	virtual void BeginPlay() override;
	// 실제 ProgressBar에 반영될 현재 퍼센트(0~1)
	float CurrentHPPercent = 1.f;
	// 데미지를 입어 도달해야 할 목표 퍼센트 (0~1)
	float TargetHPPercent = 1.f;
	// 보정 속도
	UPROPERTY(EditAnywhere, Category="UI")
	float HPInterpSpeed = 5.f;
	// 에스더 게이지 관련 변수
	float CurrentEstherGauge = 0.f;
	float TargetEstherGauge = 0.f;
	UPROPERTY(EditAnywhere, Category="HUD | Esther")
	float EstherInterpSpeed = 5.f; // 보간속도
	
	UPROPERTY(EditAnywhere, Category="Esther")
	TSubclassOf<class UEstherCutsceneWidget> CutSceneWidgetClass;
	UPROPERTY()
	class UEstherCutsceneWidget* CutSceneWidget;
	
	UPROPERTY(EditAnywhere, Category="HUD | GameEnd")
	TSubclassOf<UUserWidget> GameEndWidgetClass;
	UPROPERTY()
	UUserWidget* GameEndWidget;
};
