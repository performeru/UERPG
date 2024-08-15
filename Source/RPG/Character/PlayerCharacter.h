#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/RPAnimationAttackInterface.h"
#include "PlayerCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterInfo()
	{
		CharacterName = FText::FromString("Name");
		CharacterLevel = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharacterLevel;

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
	TObjectPtr<class UCameraComponent> FollowCmaera;
protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();

	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class URPCharacterStatComponent> Stat;
protected:
	// Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();
	float DeadEventDelayTime = 2.0f;
	void PlayDeadAnimation();
};