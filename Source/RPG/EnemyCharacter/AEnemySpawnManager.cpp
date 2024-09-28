// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/AEnemySpawnManager.h"
#include "EnemyCharacter/EnemyCharacterBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize spawn delay
	SpawnDelay = 3.0f;

	// Initialize spawn location (예: (X=0, Y=0, Z=0) 위치에 스폰)
	SpawnLocation = FVector(12200.0f, 550.0f, 1060.0f); // 원하는 위치로 변경
}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemy();
	
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnManager::SpawnEnemy()
{
	// Check if the EnemyClass is set
	if (EnemyClass)
	{
		// Set up spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the enemy at the defined location
		AEnemyCharacterBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedEnemy)
		{
			// Bind the OnDestroyed event to handle enemy death
			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AEnemySpawnManager::OnEnemyDeath);
		}
	}
	
}

void AEnemySpawnManager::OnEnemyDeath(AActor* DestroyedActor)
{
	// Set a timer to respawn the enemy after a delay
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnManager::SpawnEnemy, SpawnDelay, false);

}

