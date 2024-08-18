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

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float HealthRatio);

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> CharacterHpInfoWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<class UCharacterHpInfoWidget> CharacterHpInfoWidget;
};
