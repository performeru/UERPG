// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItem : uint8
{
	Weapon,
	Potion
};


/**
 * 
 */
UCLASS()
class RPG_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EItem Type;

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemDataAsset", GetFName());
	}
};
