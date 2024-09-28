// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AEnemySpawnManager.generated.h"

UCLASS()
class RPG_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to Spawn Enemy
	UFUNCTION()
	void SpawnEnemy();

	// Function to Handle Enemy Death
	UFUNCTION()
	void OnEnemyDeath(AActor* DestroyedActor);

private:
	// The class type of the enemy to spawn
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	TSubclassOf<class AEnemyCharacterBase> EnemyClass;

	// Spawn Location
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	FVector SpawnLocation;

	// Spawn Delay
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	float SpawnDelay;

	// Timer handle for delayed spawn
	FTimerHandle SpawnTimerHandle;

};
