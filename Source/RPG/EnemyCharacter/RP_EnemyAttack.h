// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RP_EnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URP_EnemyAttack : public UDataAsset
{
	GENERATED_BODY()
	
public:
	URP_EnemyAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	uint8 MaxAttackCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	float FrameRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboData")
	TArray<float> EffectiveFrameCount;

};
