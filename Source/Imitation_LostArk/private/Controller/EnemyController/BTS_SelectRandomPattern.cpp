


#include "Controller/EnemyController/BTS_SelectRandomPattern.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Logging/StructuredLogFormat.h"

UBTS_SelectRandomPattern::UBTS_SelectRandomPattern()
{
	NodeName = TEXT("Select Random Pattern");
	bNotifyTick = false;
}

void UBTS_SelectRandomPattern::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	
	UBlackboardComponent* BB = SearchData.OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		// 0 부터 MaxPatterns - 1 사이의 숫자를 랜덤으로 생성
		int32 RandomIndex = FMath::RandRange(0, MaxPatterns - 1);
		// 블랙보드에 값 저장
		BB->SetValueAsInt(TEXT("PatternIndex"), RandomIndex);
		UE_LOG(LogTemp, Warning, TEXT("Selected Attack Pattern : %d"), RandomIndex);
	}
}
