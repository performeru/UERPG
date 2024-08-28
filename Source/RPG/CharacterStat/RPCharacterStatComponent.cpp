// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/RPCharacterStatComponent.h"
#include "RPCharacterStatComponent.h"

// Sets default values for this component's properties
URPCharacterStatComponent::URPCharacterStatComponent()
{
	MaxHp = 200.0f;
	AttackPower = 20.0f;
	AttackPowerIncreasePerLevel = 5.0f; // ������ �� ������ ���ݷ� ��

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
	AttackPower += AttackPowerIncreasePerLevel; // ���ݷ� ����
	SetHp(MaxHp);                            // ������ �� ü���� �ִ�ġ�� ȸ��
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



