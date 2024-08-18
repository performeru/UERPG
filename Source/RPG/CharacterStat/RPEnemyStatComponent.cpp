// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/RPEnemyStatComponent.h"

// Sets default values for this component's properties
URPEnemyStatComponent::URPEnemyStatComponent()
{
	EnemyMaxHp = 200.0f;
	EnemyCurrentHp = EnemyMaxHp;
}
// Called when the game starts
void URPEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	EnemySetHp(EnemyMaxHp);
}

void URPEnemyStatComponent::EnemySetHp(float NewEnemyHp)
{
	EnemyCurrentHp = FMath::Clamp<float>(NewEnemyHp, 0.0f, EnemyMaxHp);

	OnEnemyHpChanged.Broadcast(EnemyCurrentHp);
}


float URPEnemyStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = EnemyCurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	EnemySetHp(PrevHp - ActualDamage);
	if(EnemyCurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnEnemyHpZero.Broadcast();
	}
	return ActualDamage;
}





