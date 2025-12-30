


#include "Enemy/LostArk_Enemy.h"



ALostArk_Enemy::ALostArk_Enemy()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ALostArk_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

float ALostArk_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit Reaming Health : %.f"), 100.f - ActualDamage);
	
	
	return ActualDamage;
}


void ALostArk_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALostArk_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

