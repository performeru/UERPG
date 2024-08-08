// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyCharacterBase.h"
#include "MushroomCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMushroomCharacter : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AMushroomCharacter();

protected:
	void SetDead() override;
	virtual void PlayDeadAnimation() override;
	virtual void NotifyAttackActionEnd() override;
};
