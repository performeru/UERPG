// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPAIAllInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPAIAllInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterEnemyAttackFinished);
/**
 * 
 */
class RPG_API IRPAIAllInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	
	virtual void AttackByAI() = 0;
	virtual void SetAIAttackDelegate(const FAICharacterEnemyAttackFinished& InOnAttackFinished) = 0;
};
