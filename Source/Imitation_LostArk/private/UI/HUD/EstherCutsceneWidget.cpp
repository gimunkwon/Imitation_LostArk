// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/EstherCutsceneWidget.h"

#include "MediaPlayer.h"
#include "UI/HUD/LostArk_HUD.h"


void UEstherCutsceneWidget::PlayEstherrVided()
{
	if (SilianMediaPlayer && SilianMediaSource)
	{
		// 기존 상태를 완전히 리셋
		if (SilianMediaPlayer->IsPlaying())
		{
			SilianMediaPlayer->Close();
		}
		
		
		// 영상이 끝나면 실행될 함수 연결
		// 기존에 연결된게 있을 수있으니 Clear 후 다시 Add하는게 안전
		SilianMediaPlayer->OnEndReached.Clear();
		SilianMediaPlayer->OnEndReached.AddDynamic(this, &UEstherCutsceneWidget::OnVideoFinished);
		
		SilianMediaPlayer->OnMediaOpened.Clear();
		SilianMediaPlayer->OnMediaOpened.AddDynamic(this, &UEstherCutsceneWidget::OnVideoOpened);
		
		// 소스열기
		if (SilianMediaPlayer->OpenSource(SilianMediaSource))
		{
			UE_LOG(LogTemp, Error, TEXT("실리안 영상 열기 성공"))
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("실리안 영상 열기 실패!"));
		}
		
	}
}

void UEstherCutsceneWidget::OnVideoFinished()
{
	// 미디어 플레이어 정지 및 닫기
	if (SilianMediaPlayer)
	{
		SilianMediaPlayer->OnEndReached.Clear(); // 델리게이트 해제
		SilianMediaPlayer->Close();
	}
	// HUD에게 알려서 포인터를 비워줌
	if (APlayerController* PC = GetOwningPlayer())
	{
		UE_LOG(LogTemp, Error, TEXT("HUD Is settings null"));
		if (ALostArk_HUD* HUD = Cast<ALostArk_HUD>(PC->GetHUD()))
		{
			if (HUD)
			{
				HUD->ClearCutSceneWidget();
			}			
		}
		
	}
	
	RemoveFromParent();
}
// 영상이 준비 완료 되면 여기서 재생 시작
void UEstherCutsceneWidget::OnVideoOpened(FString OpenedUrl)
{
	UE_LOG(LogTemp, Error, TEXT("OnvideonOpended 호출됨! 영상재생 시작"))
	if (SilianMediaPlayer)
	{
		SilianMediaPlayer->Rewind(); // 혹시 모르니 맨 앞으로 돌리기
		SilianMediaPlayer->Play();
	}
}
