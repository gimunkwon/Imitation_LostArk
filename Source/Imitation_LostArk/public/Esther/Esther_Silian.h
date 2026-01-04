
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Esther_Silian.generated.h"

class UNiagaraSystem;

UCLASS()
class IMITATION_LOSTARK_API AEsther_Silian : public ACharacter
{
	GENERATED_BODY()

public:
	AEsther_Silian();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	
	// 재생할 공격 몽타주
	UPROPERTY(EditAnywhere, Category="Esther")
	UAnimMontage* SlashMontage;
	// 검기 나이아가라 이펙트
	UPROPERTY(EditAnywhere, Category="Esther")
	UNiagaraSystem* SwordWaveFX;
	// 대사 사운드
	UPROPERTY(EditAnywhere, Category="Esther | Sound")
	USoundBase* EstherVoice;
	// 칼을 내리칠때 콰앙 하는소리
	UPROPERTY(EditAnywhere, Category="Esther | Sound")
	USoundBase* SlashSound;
	// 이미 데미지를 줬는지 확인하는 플래그
	bool bHasDelthDamage = false;
public:
	// 실제로 검기를 쏘는 함수
	UFUNCTION(BlueprintCallable)
	void FireSwordWave();
};
