// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHpInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCharacterHpInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetHealthBarPercent(float Percent);

protected:
	// 위젯 블루프린트에서 바인딩할 수 있는 체력바
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
};
