


#include "Animations/CharAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 애니메이션의 주인을 미리 캐싱해둠
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}


void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwnerCharacter == nullptr) return;
	// 캐릭터의 이동 속도 계산(XY 평면상의 속도만 추출)
	FVector Velocity = OwnerCharacter->GetVelocity();
	Velocity.Z = 0;
	GroundSpeed = Velocity.Size();
	// 체공 상태 확인
	if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		bIsFalling = MovementComponent->IsFalling();
	}
}


