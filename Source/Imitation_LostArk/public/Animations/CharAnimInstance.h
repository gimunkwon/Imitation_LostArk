
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_LOSTARK_API UCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
protected:
	// 캐릭터의 현재속도 (블랜드 스페이스에서 사용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float GroundSpeed;
	// 체공 여부(점프/낙하 로직 대비)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsFalling;
	// 참조용 캐릭터 포인터
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	class ACharacter* OwnerCharacter;
};
