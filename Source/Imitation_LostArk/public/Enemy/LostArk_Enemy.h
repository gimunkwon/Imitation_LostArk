
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Enemy.generated.h"

class UNiagaraSystem;

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Enemy : public ACharacter
{
	GENERATED_BODY()
public:
	ALostArk_Enemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool GetbIsAttacking() const {return bIsAttacking;}
	void ExecuteAttack(); // 실제 데미지를 입히는 함수
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<class ALostArk_DamageText> DamageTextClass;
#pragma region stat
	UPROPERTY(EditAnywhere, Category="Stat")
	float MaxHP = 100.f;
	float CurrentHP = MaxHP;
#pragma endregion
#pragma region DieProgress
	// 사망 여부 체크
	bool bIsDead = false;
	// 사망시 실행될 함수
	virtual void Die();
	UPROPERTY(EditAnywhere, Category="Effects")
	UNiagaraSystem* DieEffect;
#pragma endregion
#pragma region Detection
	// 보스 감지 범위 컴포넌트
	UPROPERTY(VisibleAnywhere, Category="Detection")
	class USphereComponent* DetectionSphere;
	// 감지 범위 반지름
	UPROPERTY(EditAnywhere, Category="Detection")
	float DetectionRange = 1000.f;
	// 플레이어가 들어왔을 때 실행할 함수
	UFUNCTION()
	void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion
	UPROPERTY(EditAnywhere, Category="Stat")
	FString EnemeyDisplayName = TEXT("보스 몬스터");
	
	// 공격 판정 범위
	UPROPERTY(VisibleAnywhere, Category="Attack")
	class UBoxComponent* AttackCollision;
	// 공격 데미지
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDamage = 20.f;
	// 공격 주기
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackDelay = 3.f;
	// 공격 가능 여부 에디터 편집
	UPROPERTY(EditAnywhere, Category="Attack")
	bool bIsAttacking = false;
	UPROPERTY(EditAnywhere, Category="AnimMontage")
	UAnimMontage* AttackMontage;
	UFUNCTION(BlueprintCallable)
	void AttackHitCheck();
	UFUNCTION(BlueprintCallable)
	void EndAttack();
	
	FTimerHandle AttackTimerHandle;
	
	
	
	
};
