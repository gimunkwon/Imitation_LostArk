
#include "Controller/EnemyController/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/LostArk_Enemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	// TickTask를 사용할 경우 True설정
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	// 보스 캐릭터 가져오기
	ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr) return EBTNodeResult::Failed;
	
	// 블랙보드에서 타켓 가져오기
	AActor* TargetPlayer = Cast<AActor>
	(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayer")));
	
	if (TargetPlayer)
	{
		// 플레이어를 쳐다보게 설정
		AIController->SetFocus(TargetPlayer);
		// 0.2초 동안만 조준하고 그 뒤에는 회전 고정
		FTimerHandle FocusTimerHandle;
		Enemy->GetWorldTimerManager().SetTimer(FocusTimerHandle,[AIController]()
		{
			if (AIController)
			{
				// 조준 해제
				AIController->ClearFocus(EAIFocusPriority::Gameplay);
			}
		}, 0.2f, false);
		
	}
	
	// 공격 중이 아닐 때만 공격 실행
	if (Enemy->GetbIsAttacking() == false)
	{
		Enemy->ExecuteAttack();
		return EBTNodeResult::InProgress;
		
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	// 보스 클래스에 있는 bIsAttacking 변수가 false가 되면
	if (Enemy->GetbIsAttacking() == false)
	{
		// 비로소 테스크를 성공으로 끝냄 다음노드로 이동가능
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
