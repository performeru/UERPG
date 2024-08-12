// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RPGGameMode.h"
#include "Blueprint/UserWidget.h"


void ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


ARPGGameMode::ARPGGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Character/BP_Player.BP_Player_C'"));
	if(DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/HUD.HUD'"));
	if (HUDWidgetClassFinder.Succeeded())
	{
		HUDWidget = HUDWidgetClassFinder.Class;
	}
}