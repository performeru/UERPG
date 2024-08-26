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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	float HpAmount = 50.0f;  // 포션 회복량을 포함하는 변수

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemDataAsset", GetFName());
	}
};
