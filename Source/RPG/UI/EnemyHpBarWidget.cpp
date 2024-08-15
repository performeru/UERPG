// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHpBarWidget.h"
#include "Components/ProgressBar.h"

UEnemyHpBarWidget::UEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnemyMaxHp = -1.0f;
}

void UEnemyHpBarWidget::UpdateEnemyHpBar(float NewEnemyCurrentHp)
{
	ensure(EnemyMaxHp > 0.0f);
	if(EnemyHpProgressBar)
	{
		EnemyHpProgressBar->SetPercent(NewEnemyCurrentHp / EnemyMaxHp);
	}

}

void UEnemyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnemyHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(EnemyHpProgressBar);
}
