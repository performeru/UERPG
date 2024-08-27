// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ExperienceWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UExperienceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UExperienceWidget(const FObjectInitializer& ObjectInitializer);
public:
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void UpdateExperience(float CurrentXP, float XPToNextLevel);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void SetLevel(int32 NewLevel); // 레벨을 업데이트하는 함수

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExperienceProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;
	
};
