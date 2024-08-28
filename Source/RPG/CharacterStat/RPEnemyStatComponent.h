// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyHpChangedDelegate, float /*EnemyCurrentHp*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPEnemyStatComponent();
public:
	FOnEnemyHpZeroDelegate OnEnemyHpZero;
	FOnEnemyHpChangedDelegate OnEnemyHpChanged;

	void SetEnemyHp(float NewHp);
	void SetEnemyCurrentHp(float NewCurrentHp);
	void SetEnemyLevel(float NewLevel);

	FORCEINLINE float EnemyGetHp() { return EnemyHp; }
	FORCEINLINE float EnemyGetCurrentHp() { return EnemyCurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	void EnemySetHp(float NewEnemyHp);

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float EnemyHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowAbstract = "true"))
	float EnemyCurrentHp;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float EnemyLevel;

};
