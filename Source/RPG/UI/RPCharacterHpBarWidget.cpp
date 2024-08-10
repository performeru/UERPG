// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPCharacterHpBarWidget.h"
#include "Components/ProgressBar.h"

URPCharacterHpBarWidget::URPCharacterHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void URPCharacterHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHpBar")));
	ensure(HpProgressBar);
}

void URPCharacterHpBarWidget::UpdateHpbar(float NewCurrentHp)
{
	ensure(MaxHp > 0);
	if(HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
