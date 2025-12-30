#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LostArk_DamageText.generated.h"

class UWidgetComponent;

UCLASS()
class IMITATION_LOSTARK_API ALostArk_DamageText : public AActor
{
	GENERATED_BODY()

public:
	ALostArk_DamageText();
	virtual void Tick(float DeltaTime) override;
	
	// 숫자를 세팅할 함수
	UFUNCTION(BlueprintImplementableEvent, Category="Damage")
	void SetDamageValue(float Damage);
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* DamageWidget;
	// 숫자가 떠 있는 시간
	UPROPERTY(EditAnywhere, Category="Damage")
	float LifeTime = 1.f;

	
};
