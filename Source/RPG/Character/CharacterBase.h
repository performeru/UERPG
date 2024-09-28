// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

/**
 * ACharacterBase serves as a base class for characters with combo actions.
 * It includes properties and methods for handling combo attacks and animation montages.
 */
UCLASS()
class RPG_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor for ACharacterBase
	ACharacterBase();

	// Called to bind functionality to input actions
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when the character is spawned
	virtual void BeginPlay() override;

protected:
	// Animation montage for combo actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// Data asset containing combo action information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboActionDataAsset> ComboActionData;

	// Process combo commands based on input
	void ProcessComboCommand();

	// Begin the combo action sequence
	void ComboActionBegin();

	// End the combo action sequence
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// Set a timer to check for combo actions
	void SetComboCheckTimer();

	// Check if a combo action is currently valid
	void ComboCheck();

	// Current combo index
	int32 CurrentCombo = 0;

	// Handle for managing the combo check timer
	FTimerHandle ComboTimerHandle;

	// Flag indicating if there is a next combo command
	bool HasNextComboCommand = false;
};

