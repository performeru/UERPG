// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPUserWidget.h"
#include "EnemyHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UEnemyHpBarWidget : public URPUserWidget
{
	GENERATED_BODY()
	
public:
	UEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void EnemySetMaxHp(float NewEnemyMaxHp) { EnemyMaxHp = NewEnemyMaxHp; }
	void UpdateEnemyHpBar(float NewEnemyCurrentHp);
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> EnemyHpProgressBar;

	UPROPERTY()
	float EnemyMaxHp;
};
