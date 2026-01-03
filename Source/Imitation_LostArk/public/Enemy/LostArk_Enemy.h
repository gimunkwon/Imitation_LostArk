
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Enemy.generated.h"
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Normal, // 평상시 (순찰 ,추적, 공격등)
	Counterable, // 카운터 가능 상태 (보스가 특정 공격 중일때)
	Groggy, // 카운터 당해서 무력하된 상태
	Dead //사망
};

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Enemy : public ACharacter
{
	GENERATED_BODY()
public:
	ALostArk_Enemy();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<class ALostArk_DamageText> DamageTextClass;
	UPROPERTY(EditAnywhere, Category="Combat")
	UMaterialInterface* IndicatorMaterial;
	// 현재 생성된 장판을 저장할 변수
	UPROPERTY()
	UDecalComponent* CurrentIndicator;
#pragma region Sound&Effect
	UPROPERTY(EditAnywhere, Category="Effects")
	UMaterialInterface* CounterOverlayMaterial;
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* CounterSound;
#pragma endregion
#pragma region stat
	UPROPERTY(EditAnywhere, Category="Stat")
	float MaxHP = 100.f;
	UPROPERTY(EditAnywhere, Category="Stat")
	FString EnemeyDisplayName = TEXT("보스 몬스터");
	float CurrentHP = MaxHP;
	using enum EEnemyState;
	UPROPERTY(VisibleAnywhere, Category="State")
	EEnemyState CurrentState = EEnemyState::Normal;
	UPROPERTY(EditAnywhere, Category="State")
	float GroggyDuration = 3.f; // 무력화 시간
	FTimerHandle GroggyTimerHandle;
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
#pragma region Attack
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
	// 몽타주들을 담을 배열
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<UAnimMontage*> AttackPatternMontages;
	
	FTimerHandle AttackTimerHandle;
#pragma endregion
private:
	int32 CurrentPatternIndex = 0;
public:
	bool GetbIsAttacking() const {return bIsAttacking;}
	void ExecuteAttack(int32 PatternIndex); // 실제 데미지를 입히는 함수
	// 상태 변경 함수
	UFUNCTION(BlueprintCallable)
	void SetEnemyState(EEnemyState Newstate) {CurrentState = Newstate;}
	UFUNCTION(BlueprintCallable)
	EEnemyState GetEnemyState() const {return CurrentState;}
	// 카운터 판정은 위한 Getter
	bool GetIsCounterable() const {return CurrentState == EEnemyState::Counterable;}
	// 카운터 성공시 호출 될 함수
	UFUNCTION(BlueprintCallable)
	void OnCounterSucces();
	// 무력화 종료후 다시 복귀하는 함수
	void ResetFromGroggy();
	UFUNCTION(BlueprintCallable)
	void CounterStart();
	UFUNCTION(BlueprintCallable)
	void CounterEnd();
	// 장판을 켜고 끄는 함수
	UFUNCTION(BlueprintCallable)
	void ShowIndicator();
	UFUNCTION(BlueprintCallable)
	void HideIndicator();
};
