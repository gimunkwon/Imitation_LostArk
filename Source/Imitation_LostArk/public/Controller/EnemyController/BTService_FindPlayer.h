

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindPlayer.generated.h"

UCLASS()
class IMITATION_LOSTARK_API UBTService_FindPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_FindPlayer();
	// 주기적으로 실행될 함수
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, Category="AI")
	float DetectionRange = 1000.f; // 인식범위
	UPROPERTY(EditAnywhere, Category="AI")
	float AttackRange = 250.f; // 공격 범위
};
