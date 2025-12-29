


#include "Animations/CharAnimInstance.h"

#include "GameFramework/Character.h"

void UCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 애니메이션의 주인을 미리 캐싱해둠
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}


void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}


