// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RPHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/CharacterHpInfoWidget.h"
#include "Character/PlayerCharacter.h"
#include "CharacterStat/RPCharacterStatComponent.h"


ARPHUD::ARPHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>CharacterHpInfoWidgetRef(TEXT("/Game/UI/UI_Character.UI_Character_C"));
	if (CharacterHpInfoWidgetRef.Succeeded())
	{
		CharacterHpInfoWidgetClass = CharacterHpInfoWidgetRef.Class;
	}

}

void ARPHUD::BeginPlay()
{
    Super::BeginPlay();

	if (CharacterHpInfoWidgetClass)
	{
		CharacterHpInfoWidget = CreateWidget<UCharacterHpInfoWidget>(GetWorld(), CharacterHpInfoWidgetClass);

		if(CharacterHpInfoWidget)
		{
			CharacterHpInfoWidget->AddToViewport();

			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn()))
			{
				PlayerCharacter->CharacterHpInfoWidget = CharacterHpInfoWidget;

				URPCharacterStatComponent* Stat = PlayerCharacter->GetStat();
				if (Stat)
				{
					PlayerCharacter->UpdateHealthUI(Stat->GetCurrentHp());
				}
			}
		}
	}
}

void ARPHUD::UpdateHealthBar(float HealthRatio)
{
	if (CharacterHpInfoWidget)
	{
		CharacterHpInfoWidget->SetHealthBarPercent(HealthRatio);
	}
}
