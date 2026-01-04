

#include "GameMode/Imitation_LostArk_GameMode.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AImitation_LostArk_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 배경음악 재생
	if (BackgroundMusic)
	{
		PlayBGM(BackgroundMusic);
	}
}

void AImitation_LostArk_GameMode::PlayBGM(USoundBase* NewMusic, bool bFadeIn)
{
	if (!NewMusic) return;
	// 이미 다른 음악이 나오고 있다면 멈춤
	if (MusicComponent && MusicComponent->IsPlaying())
	{
		MusicComponent->Stop();
	}
	// 2D 사운드(화면 전체에 들리는 배경음)로 재생
	if (bFadeIn)
	{
		MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), NewMusic
			, 1.f, 1.f,0.f, nullptr, true);
		if (MusicComponent)
		{
			MusicComponent->FadeIn(2.0f); // 2초간 서서히커짐
		}
	}
	else
	{
		MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), NewMusic);
	}
}

void AImitation_LostArk_GameMode::StopBGM(float FadeOutTime)
{
	if (MusicComponent && MusicComponent->IsPlaying())
	{
		MusicComponent->FadeOut(FadeOutTime, 0.0f);
	}
}
