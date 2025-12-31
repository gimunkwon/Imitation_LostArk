

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArk_Player.generated.h"

class UNiagaraSystem;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class IMITATION_LOSTARK_API ALostArk_Player : public ACharacter
{
	GENERATED_BODY()
public:
	ALostArk_Player();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// 무기 메쉬
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#pragma region CameraComponent
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* TopDownCamera;
#pragma endregion
#pragma region CharacterRotate
	// 회전속도
	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationSpeed = 10.f;
	// 마우스 방향으로 캐릭터를 회전시키는 함수
	void SmoothRotateToCursor(float DeltaTime);
#pragma endregion
#pragma region CharacterDash
	UPROPERTY(EditAnywhere, Category="Movement | Dash")
	float DashImpulse = 2000.f; // 대시강도
	UPROPERTY(EditAnywhere, Category="Movement | Dash")
	float DashCoolDown = 2.f; // 쿨타임(초)
	
	bool bCanDash = true; // 대시 가능 여부 플래그
	FTimerHandle DashTimerHandle; // 쿨타임 관리를 위한 타이머
#pragma endregion
#pragma region NiagaraEffects
	// 대시 시작시 발동할 나이아가라 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	UNiagaraSystem* DashStartEffects;
#pragma endregion
#pragma region Attack
	// 공격 종료 처리
	UFUNCTION(BlueprintCallable) // 애니메이션 노티파이에서 호출 가능하도록
	void EndAttack();
	UFUNCTION(BlueprintCallable)
	void AttackHitCheck();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animaiton")
	UAnimMontage* AttackMontage;
	int32 CurrentCombo = 0; // 현재 몇타수 인지
	bool bSaveCombo = false; // 다음 콤보를 입력했는지 여부
#pragma endregion
#pragma region Stat
	UPROPERTY(EditAnywhere, Category="Stat")
	float MaxHP = 200.f;
	UPROPERTY(VisibleAnywhere, Category="Stat")
	float CurrentHP;
#pragma endregion
	
public:	
	// 카메라 컴포넌트 접근용
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetTopDownCamera() const { return TopDownCamera; }
	// 컨트롤러에서 입력 상태를 전달받기 위한 함수
	void SetInputDirectionMode(bool bIsPressed){bIsInputHold = bIsPressed;}
	// 대시 입력시 호출 될 함수
	void Dash();
	// 대시 쿨타임 초기화 함수
	void ResetDash();
	// 공격 실행 함수
	void Attack();
	UFUNCTION(BlueprintCallable, Category="Attack")
	void ComboCheck(); // 콤보 타이밍 체크
	void EndCombo(); // 콤보 초기화
private:
	bool bIsInputHold = false;
	bool bIsAttacking = false;
	
};

