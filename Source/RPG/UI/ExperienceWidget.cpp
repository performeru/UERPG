// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ExperienceWidget.h"
#include "ExperienceWidget.h"
#include "Components/ProgressBar.h"


UExperienceWidget::UExperienceWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UExperienceWidget::UpdateExperience(float CurrentXP, float XPToNextLevel)
{
	if (ExperienceProgressBar)
	{
		float Progress = CurrentXP / XPToNextLevel;
		ExperienceProgressBar->SetPercent(Progress);
	}

}

void UExperienceWidget::SetLevel(int32 NewLevel)
{
	if (LevelText)
	{
		FString LevelTextString = FString::Printf(TEXT("Level: %d"), NewLevel);
		LevelText->SetText(FText::FromString(LevelTextString));
	}
}