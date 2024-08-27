#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "Interface/RPAnimationAttackInterface.h"
#include "Interface/ItemInterface.h"
#include "PlayerCharacter.generated.h"

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

	FTakeItemDelegate(){}
	FTakeItemDelegate(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}

	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class RPG_API APlayerCharacter : public ACharacterBase, public IRPAnimationAttackInterface, public IItemInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float LookSensitivity = 1.0f;
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

protected:
	// Item Section
	UPROPERTY()
	TArray<FTakeItemDelegate> TakeItemAction;

	virtual void TakeItem(class UItemDataAsset* InItemData) override;
	virtual void DrinkPotion(class UItemDataAsset* InItemData);
	virtual void EquipWeapon(class UItemDataAsset* InItemData);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

protected:
	// 레벨 섹션
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

	// 경험치 UI 업데이트 함수
	void UpdateExperienceUI();

public:
	// 경험치와 레벨 관련 함수
	void GainExperience(float Amount);
	void LevelUp();
};