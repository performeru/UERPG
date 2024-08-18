// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHpBarWidget.h"
#include "Components/ProgressBar.h"

UCharacterHpBarWidget::UCharacterHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CharacterMaxHp = -1.0f;
}

void UCharacterHpBarWidget::UpdateCharacterHpBar(float NewCurrentCharacterHp)
{
	ensure(CharacterMaxHp > 0.0f);
	if (CharacterHpProgressBar)
	{
		CharacterHpProgressBar->SetPercent(NewCurrentCharacterHp / CharacterMaxHp);
	}
}

void UCharacterHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(CharacterHpProgressBar);

}
