// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHpInfoWidget.h"
#include "Components/ProgressBar.h"

void UCharacterHpInfoWidget::SetHealthBarPercent(float Percent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(Percent);
    }
    
}
