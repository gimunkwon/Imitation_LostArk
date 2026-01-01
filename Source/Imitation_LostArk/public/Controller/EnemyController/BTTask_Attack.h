
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"


UCLASS()
class IMITATION_LOSTARK_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Attack();
protected:
	// 테스크가 실행 될때 호출되는 핵심 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// 테스크가 진행 중일때 매 프레임 호출
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	// 공격 애니메이션이 끝날 때 까지 기다릴지 여부등을 관리하기 위한 변수
	bool bIsAttacking = false;
};
