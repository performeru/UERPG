#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/RPAnimationAttackInterface.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class RPG_API APlayerCharacter : public ACharacterBase, public IRPAnimationAttackInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PostInitializeComponents() override;

public:
	virtual void BeginPlay() override;
protected:
	// Camera

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;
protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCharacterControlData(const class UCharacterControlDataAsset* CharacterControlData);
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UCharacterControlDataAsset*> CharacterControlManager;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void QuaterMove(const FInputActionValue& Value);
	void Attack();

	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class URPCharacterStatComponent> Stat;
public:
	// Getter for Stat
	UFUNCTION(BlueprintCallable, Category = "Stat")
	URPCharacterStatComponent* GetStat() const { return Stat; }
	
	// Health UI
	UPROPERTY()
	TObjectPtr<class UCharacterHpInfoWidget> CharacterHpInfoWidget;

	void UpdateHealthUI(float CurrentHealth);
protected:
	// Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();
	float DeadEventDelayTime = 2.0f;
	void PlayDeadAnimation();

};