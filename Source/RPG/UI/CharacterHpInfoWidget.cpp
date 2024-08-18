// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHpInfoWidget.h"
#include "Components/ProgressBar.h"

void UCharacterHpInfoWidget::SetHealthBarPercent(float Percent)
{
	/*if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}*/
    if (HealthBar)
    {
        HealthBar->SetPercent(Percent);
        UE_LOG(LogTemp, Warning, TEXT("Health Bar Percent Set: %f"), Percent); // 디버그 로그
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HealthBar is not valid!"));
    }
}
