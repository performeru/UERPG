// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/RPCharacterStatComponent.h"
#include "RPCharacterStatComponent.h"

// Sets default values for this component's properties
URPCharacterStatComponent::URPCharacterStatComponent()
{
	MaxHp = 200.0f;
	AttackPower = 20.0f;
	AttackPowerIncreasePerLevel = 5.0f; // 레벨업 시 증가할 공격력 양

	SetHp(MaxHp);

	bWantsInitializeComponent = true;

}

// Called when the game starts
void URPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;

}

float URPCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0 ,InDamage);

	SetHp(PrevHp - ActualDamage);
	if(CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void URPCharacterStatComponent::LevelUpStats()
{
	AttackPower += AttackPowerIncreasePerLevel; // 공격력 증가
	SetHp(MaxHp);                            // 레벨업 시 체력을 최대치로 회복
}

float URPCharacterStatComponent::GetAttackPower() 
{
	return AttackPower;
}

void URPCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);

}



