// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMyBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTask_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
