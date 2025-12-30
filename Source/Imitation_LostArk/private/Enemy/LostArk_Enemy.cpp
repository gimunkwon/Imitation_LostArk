


#include "Enemy/LostArk_Enemy.h"

#include "UI/LostArk_DamageText.h"


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
	
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.f, MaxHP);
	
	// 데미지 숫자를 띄울 위치
	if (DamageTextClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(50.f, 50.f, 80.f);
	
		// 약간의 랜덤성을 주기
		// SpawnLocation.X = FMath::RandRange(-20.f, 20.f);
		// SpawnLocation.Y = FMath::RandRange(-20.f, 20.f);
	
		// DamageText 액터를 스폰
		ALostArk_DamageText* DamageActor = GetWorld()->SpawnActor<ALostArk_DamageText>
		(DamageTextClass, SpawnLocation, FRotator::ZeroRotator);
		if (DamageActor)
		{
			DamageActor->SetDamageValue(ActualDamage);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit Reaming Health : %f"), CurrentHP);
	
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

