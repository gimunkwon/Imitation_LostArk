

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LostArk_AIController.generated.h"

UCLASS()
class IMITATION_LOSTARK_API ALostArk_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ALostArk_AIController();
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override; // 캐릭터를 조종하기 시작할때 호출
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category="AI")
	class UBehaviorTree* BTAsset;
	UPROPERTY(EditAnywhere, Category="AI")
	class UBlackboardData* BBAsset;

	
};
