
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SelectRandomPattern.generated.h"

UCLASS()
class IMITATION_LOSTARK_API UBTS_SelectRandomPattern : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_SelectRandomPattern();
protected:
	// 서비스가 속한 노드가 실행될때 호출
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
private:
	// 블랙보드의 PatternIndex 키를 선택하기 위한 변수
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector PatternIndexKey;
	// 공격 패턴의 총 개수
	UPROPERTY(EditAnywhere, Category="Combat")
	int32 MaxPatterns = 3;
	
};
