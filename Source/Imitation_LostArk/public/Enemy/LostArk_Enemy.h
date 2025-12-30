

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Enemy.generated.h"

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	ALostArk_Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	
};
