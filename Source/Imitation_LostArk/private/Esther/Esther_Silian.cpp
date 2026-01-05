
#include "Esther/Esther_Silian.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AEsther_Silian::AEsther_Silian()
{
	
	PrimaryActorTick.bCanEverTick = false;
	if (GetCapsuleComponent())
	{
		// 캡슐이 다른 캐릭터(Pawn)를 무시하도록 설정
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
	// 메시도 충돌을 끔
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEsther_Silian::BeginPlay()
{
	Super::BeginPlay();
	if (EstherVoice)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EstherVoice);
	}
	
	if (SlashMontage)
	{
		float Duration = PlayAnimMontage(SlashMontage);
		
		SetLifeSpan(Duration + 0.1f);
	}
}

void AEsther_Silian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEsther_Silian::FireSwordWave()
{
	// 이미 실행되었다면 즉시 리턴
	if (bHasDelthDamage) return;
	bHasDelthDamage = true;
	
	UE_LOG(LogTemp ,Error, TEXT("Silian [%p] FireSwordWave Called"),this);
	
	// 화려한 검기 나이아가라 스폰
	if (SwordWaveFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SwordWaveFX, GetActorLocation(),
			GetActorRotation());
	}
	// 검기 효과음
	if (SlashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation());
	}
	// 실제 데미지 판정
	FVector Start = GetActorLocation() + GetActorForwardVector() * 100.f;
	FVector End = Start + GetActorForwardVector() * 1500.f;
	
	TArray<FHitResult> Hits;
	FVector BoxHalfSize = FVector(100.f, 400.f, 400.f);
	
	bool bHit = UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(), Start, End, BoxHalfSize, GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
		{this, GetOwner()}, EDrawDebugTrace::ForDuration, Hits, true);
	
	// 중복 타격을 방지하기 위한 집합 (Tset은 중복을 허용하지 않음)
	TSet<AActor*> AlreadyDamagedActors;
	
	if (bHit)
	{
		for (const FHitResult& Hit : Hits)
		{
			AActor* HitActor = Hit.GetActor();
			
			if (HitActor && !AlreadyDamagedActors.Contains(HitActor))
			{
				UGameplayStatics::ApplyDamage(HitActor, 100.f, GetInstigatorController(),
				this, nullptr);
				AlreadyDamagedActors.Add(HitActor);
				
				UE_LOG(LogTemp, Warning, TEXT("Damage Applied to : %s"), *HitActor->GetName());
			}
			
		}
	}
}




