

#include "Enemy/Animations/LostArk_Enemy_Animinstance.h"

#include "GameFramework/Character.h"

void ULostArk_Enemy_Animinstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void ULostArk_Enemy_Animinstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwnerCharacter == nullptr) return;
	
	FVector Velocity = OwnerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	GroundSpeed = Velocity.Size();
}
