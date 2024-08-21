// Fill out your copyright notice in the Description page of Project Settings.


#include "Control/RPPlayerController.h"

void ARPPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameOnly GameOnlyInputMode;
    SetInputMode(GameOnlyInputMode);

    bShowMouseCursor = false;

    SetIgnoreLookInput(false);
    SetIgnoreMoveInput(false);

}