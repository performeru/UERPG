// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPCharacterStatComponent();
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

	void UpdateHp(float NewHp) { SetHp(NewHp); }
public:
	// 레벨업 시 호출될 함수
	void LevelUpStats();
	// 공격력을 반환하는 함수
	float GetAttackPower();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float AttackPower;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float HpIncreasePerLevel;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float AttackPowerIncreasePerLevel;

};
