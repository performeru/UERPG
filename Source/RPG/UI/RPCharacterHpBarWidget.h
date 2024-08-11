// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPUserWidget.h"
#include "RPCharacterHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPCharacterHpBarWidget : public URPUserWidget
{
	GENERATED_BODY()
	
public:
	URPCharacterHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpbar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;


};
