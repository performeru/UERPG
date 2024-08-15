// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RPGGameMode.h"
#include "Blueprint/UserWidget.h"


void ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

}


ARPGGameMode::ARPGGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Character/BP_Player.BP_Player_C'"));
	if(DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

}