// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ARPHUD();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCharacterHpBarWidget> CharacterHpBarWidgetClass;

	UPROPERTY()
	UCharacterHpBarWidget* CharacterHpBarWidget;
};
