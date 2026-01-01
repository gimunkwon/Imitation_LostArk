
#include "Controller/EnemyController/LostArk_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"


ALostArk_AIController::ALostArk_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALostArk_AIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALostArk_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALostArk_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// 비헤이비어 트리와 블랙보드 실행
	if (BBAsset && BTAsset)
	{
		// 1. Tobjectptr인 멤버 변수 Blackboard를 직접 넣지 않고
		// 일반 포인터 변수를 하나 선언해서 넘겨줌
		UBlackboardComponent* BlackboardComp = nullptr;
		// 2. UseBlackboard는 성공하면 내부적으로 BlackboradComp에 컴포넌트를 할당해줌
		if (UseBlackboard(BBAsset, BlackboardComp))
		{
			// 3. 월드의 첫 번째 플레이어를 타겟으로 지정
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				// 4. 플레이어가 조종 중인 캐릭터가 있는지 확인
				AActor* LostArkPlayer = PC->GetPawn();
				if (LostArkPlayer)
				{
					// 컴포넌트를 통해 값을 세팅
					BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), LostArkPlayer);
				}
			}
			
			// 비헤이비어 트리 실행
			RunBehaviorTree(BTAsset);
		}
	}
}

