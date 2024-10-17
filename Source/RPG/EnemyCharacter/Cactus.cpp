// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/Cactus.h"

ACactus::ACactus()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CactusMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MonsterForSurvivalGame/Mesh/PBR/Cactus_SK.Cactus_SK'"));
	if (CactusMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CactusMeshRef.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -55.0f));  // ĸ�� ������Ʈ�� ���� ���̿� ����
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // �ʿ� �� ȸ�� ����
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CactusAnimInstance(TEXT("Game/MonsterForSurvivalGame/Animation/PBR/Cactus/AB_RP_Cactus.AB_RP_Cactus_C"));
	if (CactusAnimInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(CactusAnimInstance.Class);
	}
}
