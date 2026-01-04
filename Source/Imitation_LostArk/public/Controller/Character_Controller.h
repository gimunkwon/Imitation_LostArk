

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Character_Controller.generated.h"

class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;

UCLASS()
class IMITATION_LOSTARK_API ACharacter_Controller : public APlayerController
{
	GENERATED_BODY()
public:
	ACharacter_Controller();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Dash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Attack;
	//Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Skill_Q;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Skill_W;
	//Esther Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Esther_Z; // 실리안
	
	
	// Visual effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	UNiagaraSystem* ClickEffect;
	
	
	// 입력 핸들러
	void OnMoveStarted();
	void OnMoveReleased();
	void OnDashStarted();
	void OnAttackStarted();
	// 스킬함수
	void OnSkillQStarted();
	void OnSkillWStarted();
	// 에스더 함수
	void OnEstherZStarted();
	
private:
	void MoveToMouseCursor();
	bool bIsInputPressed = false;
	bool bIsEffectOn = false;
	
};
