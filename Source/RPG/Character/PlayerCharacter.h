#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/RPAnimationAttackInterface.h"
#include "Interface/ItemInterface.h"
#include "Item/StartWeapon.h"
#include "Item/StartWeapon.h"
#include "PlayerCharacter.generated.h"

// Enums & Delegates
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UItemDataAsset* /*InItemData*/);

USTRUCT(BlueprintType)
struct FTakeItemDelegate
{
	GENERATED_BODY()

	FTakeItemDelegate() {}
	FTakeItemDelegate(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}

	FOnTakeItemDelegate ItemDelegate;
};

// Class Declaration
UCLASS()
class RPG_API APlayerCharacter : public ACharacterBase, public IRPAnimationAttackInterface, public IItemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	TObjectPtr<class AStartWeapon> CurrentWeapon;

protected:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LookSensitivity = 1.0f;

	// Input Mapping
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

	// Character Control
	void SetCharacterControlData(const class UCharacterControlDataAsset* CharacterControlData);
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	UPROPERTY(EditAnywhere, Category = "CharacterControl", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UCharacterControlDataAsset*> CharacterControlManager;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void QuaterMove(const FInputActionValue& Value);
	void Attack();

	// Combat
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class URPCharacterStatComponent> Stat;

public:
	UFUNCTION(BlueprintCallable, Category = "Stat")
	URPCharacterStatComponent* GetStat() const { return Stat; }

	// Health UI
	UPROPERTY()
	TObjectPtr<class UCharacterHpInfoWidget> CharacterHpInfoWidget;

	void UpdateHealthUI(float CurrentHealth);

	// Death Handling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	void SetDead();
	float DeadEventDelayTime = 2.0f;
	void PlayDeadAnimation();

	// Item Handling
	UPROPERTY()
	TArray<FTakeItemDelegate> TakeItemAction;

	virtual void TakeItem(class UItemDataAsset* InItemData) override;
	void DrinkPotion(class UItemDataAsset* InItemData);
	void EquipWeapon(class UItemDataAsset* InItemData);
	void EquipWeapon(AStartWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// Leveling System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentExperience;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float ExperienceToNextLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UExperienceWidget> ExperienceWidgetClass;

	UPROPERTY()
	UExperienceWidget* ExperienceWidget;

	void UpdateExperienceUI();

public:
	void GainExperience(float Amount);
	void LevelUp();
};
