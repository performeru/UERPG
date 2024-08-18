// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCharacterHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterHpBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void SetCharacterMaxHp(float NewCharacterMaxHp) { CharacterMaxHp = NewCharacterMaxHp; }
	void UpdateCharacterHpBar(float NewCurrentCharacterHp);
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> CharacterHpProgressBar;

	UPROPERTY()
	float CharacterMaxHp;
};
