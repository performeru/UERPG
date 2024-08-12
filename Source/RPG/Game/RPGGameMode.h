// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARPGGameMode();

protected:
	virtual void BeginPlay() override;
	// 수정부분 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> HUDWidget;
};
