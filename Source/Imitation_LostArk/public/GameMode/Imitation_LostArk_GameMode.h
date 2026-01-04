
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Imitation_LostArk_GameMode.generated.h"


UCLASS()
class IMITATION_LOSTARK_API AImitation_LostArk_GameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();
	
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* BackgroundMusic;
	// 재생 중인 음악을 제어하기 위한 컴포넌트
	UPROPERTY()
	UAudioComponent* MusicComponent;
	
public:
	// 외부(보스나 플레이어)에서 음악을 바꿀 수 있게 만든 함수
	void PlayBGM(USoundBase* NewMusic, bool bFadeIn = true);
	void StopBGM(float FadeOutTime = 1.0f);
};
