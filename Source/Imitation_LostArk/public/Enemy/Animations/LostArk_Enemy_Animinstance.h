
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LostArk_Enemy_Animinstance.generated.h"

UCLASS()
class IMITATION_LOSTARK_API ULostArk_Enemy_Animinstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
	// 참조용 캐릭터 포인터
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	class ACharacter* OwnerCharacter;
};
