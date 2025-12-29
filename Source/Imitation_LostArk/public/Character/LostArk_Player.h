

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
	virtual void Tick(float DeltaTime) override;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* TopDownCamera;
	// 회전속도
	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationSpeed = 10.f;
	// 마우스 방향으로 캐릭터를 회전시키는 함수
	void SmoothRotateToCursor(float DeltaTime);
	
public:	
	// 카메라 컴포넌트 접근용
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetTopDownCamera() const { return TopDownCamera; }
	// 컨트롤러에서 입력 상태를 전달받기 위한 함수
	void SetInputDirectionMode(bool bIsPressed){bIsInputHold = bIsPressed;}
private:
	bool bIsInputHold = false;
	
};
