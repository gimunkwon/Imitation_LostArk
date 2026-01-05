
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EstherCutsceneWidget.generated.h"

UCLASS()
class IMITATION_LOSTARK_API UEstherCutsceneWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 블루 프린트에서 미디어 플레이어 에셋을 할당
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Esther")
	class UMediaPlayer* SilianMediaPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Esther")
	class UMediaSource* SilianMediaSource;
	
public:
	void PlayEstherrVided();
	UFUNCTION()
	void OnVideoFinished();
	UFUNCTION()
	void OnVideoOpened(FString OpenedUrl);
};


