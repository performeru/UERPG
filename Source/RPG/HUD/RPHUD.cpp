// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RPHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/CharacterHpBarWidget.h"

ARPHUD::ARPHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> CharacterHpBarWidgetClassRef(TEXT("/Game/UI/CharacterHpBar.CharacterHpBar_C"));
    if (CharacterHpBarWidgetClassRef.Class)
    {
        CharacterHpBarWidgetClass = CharacterHpBarWidgetClassRef.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find CharacterHpBarWidgetClass! Check the path."));
    }

}

void ARPHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!CharacterHpBarWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterHpBarWidgetClass is nullptr! Please check if the Blueprint class is assigned correctly in the HUD."));
        return;
    }

    CharacterHpBarWidget = CreateWidget<UCharacterHpBarWidget>(GetWorld(), CharacterHpBarWidgetClass);
    if (CharacterHpBarWidget)
    {
        CharacterHpBarWidget->AddToViewport();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create CharacterHpBarWidget!"));
    }
}
