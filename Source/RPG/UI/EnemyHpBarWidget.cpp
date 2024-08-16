// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/RPEnemyWidgetInterface.h"

UEnemyHpBarWidget::UEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnemyMaxHp = -1.0f;
}

void UEnemyHpBarWidget::UpdateEnemyHpBar(float NewEnemyCurrentHp)
{
	ensure(EnemyMaxHp > 0.0f);
	if(EnemyHpProgressBar)
	{
		//EnemyHpProgressBar->SetPercent(NewEnemyCurrentHp / EnemyMaxHp);
		float Percent = NewEnemyCurrentHp / EnemyMaxHp;
		EnemyHpProgressBar->SetPercent(Percent);

		UE_LOG(LogTemp, Warning, TEXT("UpdateEnemyHpBar: NewEnemyCurrentHp = %f, Percent = %f"), NewEnemyCurrentHp, Percent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateEnemyHpBar: EnemyHpProgressBar is null"));
	}

}

void UEnemyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (!EnemyHpProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateEnemyHpBar: EnemyHpProgressBar is null"));
		return;
	}

	EnemyHpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnemyHpProgressBar")));
	ensure(EnemyHpProgressBar);

	IRPEnemyWidgetInterface* EnemyWidget = Cast<IRPEnemyWidgetInterface>(OwningActor);
	if(EnemyWidget)
	{
		EnemyWidget->SetupEnemyWidget(this);
	}
}
