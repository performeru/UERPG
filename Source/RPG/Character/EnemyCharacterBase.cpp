// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MonsterMeshRef(TEXT("/Game/MonsterForSurvivalGame/Mesh/PBR/Mushroom_SK.Mushroom_SK"));
	if(MonsterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MonsterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/BP_Mushroom.BP_Mushroom_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
