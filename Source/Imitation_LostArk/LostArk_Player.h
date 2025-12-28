

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Player.generated.h"

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Player : public ACharacter
{
	GENERATED_BODY()

public:
	
	ALostArk_Player();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
