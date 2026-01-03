


#include "Animations/LostArk_Enemy_AnimNState.h"

#include "Enemy/LostArk_Enemy.h"


void ULostArk_Enemy_AnimNState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("Notify Begin!!!"));
	if (MeshComp && MeshComp->GetOwner())
	{
		ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->CounterStart();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error ,TEXT("Notify Begin: Failed to Cast to LostArkEnemy"));
	}

}

void ULostArk_Enemy_AnimNState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(MeshComp->GetOwner());
	if (Enemy)
	{
		Enemy->OnCounterSucces();
	}
}


void ULostArk_Enemy_AnimNState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("Notify End!!!"));
	if (MeshComp && MeshComp->GetOwner())
	{
		ALostArk_Enemy* Enemy = Cast<ALostArk_Enemy>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->CounterEnd();
		}
	}
}
