// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemDataAsset.h"
#include "WeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UWeaponItemData : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemDataAsset", GetFName());
	}
};
