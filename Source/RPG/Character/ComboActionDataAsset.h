// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboActionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UComboActionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UComboActionDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	uint8 MaxComboCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
	float FrameRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComboData")
	TArray<float> EffectiveFrameCount;

};
