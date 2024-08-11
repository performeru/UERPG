// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPWidgetComponent.h"
#include "RPUserWidget.h"

void URPWidgetComponent::InitWidget()
{
	Super::InitWidget();

	URPUserWidget* RPUserWidget = Cast<URPUserWidget>(GetWidget());
	if(RPUserWidget)
	{
		RPUserWidget->SetOwningActor(GetOwner());
	}
}
