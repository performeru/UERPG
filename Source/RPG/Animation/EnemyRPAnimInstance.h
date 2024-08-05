// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/RPAnimInstance.h"
#include "EnemyRPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UEnemyRPAnimInstance : public URPAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyRPAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
