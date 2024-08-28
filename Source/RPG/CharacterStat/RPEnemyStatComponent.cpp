// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/RPEnemyStatComponent.h"

// Sets default values for this component's properties
URPEnemyStatComponent::URPEnemyStatComponent()
{
	EnemyHp = 200.0f;
	EnemyCurrentHp = EnemyHp;
}

void URPEnemyStatComponent::SetEnemyHp(float NewHp)
{
	EnemyHp = NewHp;
	EnemyCurrentHp = FMath::Clamp<float>(EnemyCurrentHp, 0.0f, EnemyHp); // 체력 초기화 시 현재 체력을 최대 체력 범위 내로 설정
}

void URPEnemyStatComponent::SetEnemyCurrentHp(float NewCurrentHp)
{
	EnemyCurrentHp = FMath::Clamp<float>(NewCurrentHp, 0.0f, EnemyHp); // 현재 체력을 최대 체력 범위 내로 설정
}

void URPEnemyStatComponent::SetEnemyLevel(float NewLevel)
{
	EnemyLevel = NewLevel;
}

// Called when the game starts
void URPEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	EnemySetHp(EnemyHp);
}

void URPEnemyStatComponent::EnemySetHp(float NewEnemyHp)
{
	EnemyCurrentHp = FMath::Clamp<float>(NewEnemyHp, 0.0f, EnemyHp);

	OnEnemyHpChanged.Broadcast(EnemyCurrentHp);
}


float URPEnemyStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = EnemyCurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);;

	EnemySetHp(PrevHp - ActualDamage);
	UE_LOG(LogTemp, Warning, TEXT("Applied Damage: %f. New HP: %f"), ActualDamage, EnemyCurrentHp);
	
	if(EnemyCurrentHp <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy HP is zero or less."));
		OnEnemyHpZero.Broadcast();
	}
	return ActualDamage;
}





