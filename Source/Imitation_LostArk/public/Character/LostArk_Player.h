

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Player.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Player : public ACharacter
{
	GENERATED_BODY()
public:
	ALostArk_Player();
protected:
	virtual void BeginPlay() override;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* TopDownCamera;
	
public:	
	// 카메라 컴포넌트 접근용
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetTopDownCamera() const { return TopDownCamera; }
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
