// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/RPAIAllInterface.h"


UMyBTTask_TurnToTarget::UMyBTTask_TurnToTarget()
{
	
}

EBTNodeResult::Type UMyBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IRPAIAllInterface* AIPawn = Cast<IRPAIAllInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AIPawn->GetAITurnSpeed();
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
