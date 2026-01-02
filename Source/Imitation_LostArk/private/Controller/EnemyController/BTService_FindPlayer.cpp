


#include "Controller/EnemyController/BTService_FindPlayer.h"

#include "AIController.h"
#include "HeadMountedDisplayTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/LostArk_Enemy.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = TEXT("Find Player");
	Interval = 0.5f; // 0.5초마다 플레이어 위치 갱신
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(PlayerPawn == nullptr) return;
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControllingPawn == nullptr) return;
	ALostArk_Enemy* Enemy  = Cast<ALostArk_Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	// 2. 거리 계산
	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), ControllingPawn->GetActorLocation());
	
	// 3. 감지 범위 로직
	if (Distance <= DetectionRange)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		// 범위 밖일때 : 공격중이면 지우지 않고 유지, 공격중이 아닐때만 지움
		if (Enemy->GetbIsAttacking() == false)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), nullptr);			
		}
		
	}
	// 공격 범위 로직
	bool bInRange = (Distance <= AttackRange);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsInAttackRange"), bInRange);
	
	// 5. 시각화
	FVector Center = ControllingPawn->GetActorLocation();
	Center.Z -= 88.f;
	
	
	// 감지 범위 그리기
	DrawDebugCircle(GetWorld(), Center, DetectionRange, 64, FColor::Green,
		false, 0.5f, 0, 2.0f
		, FVector(1,0,0),FVector(0,1,0), false);
	DrawDebugCircle(GetWorld(), Center, AttackRange, 64, FColor::Red,
		false, 0.5f, 0, 2.0f
		, FVector(1,0,0),FVector(0,1,0), false);
	
	
}
