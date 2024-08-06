// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/MushroomCharacter.h"

AMushroomCharacter::AMushroomCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MonsterForSurvivalGame/Mesh/PBR/Mushroom_SK.Mushroom_SK'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -55.0f));  // ĸ�� ������Ʈ�� ���� ���̿� ����
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // �ʿ� �� ȸ�� ����

	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/AB_RP_Mushroom.AB_RP_Mushroom_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

}

void AMushroomCharacter::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHanlde;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHanlde, FTimerDelegate::CreateLambda( [&]() { Destroy(); }), DeadEventDelayTime, false);
}
