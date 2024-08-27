// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/MushroomCharacter.h"

AMushroomCharacter::AMushroomCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MonsterForSurvivalGame/Mesh/PBR/Mushroom_SK.Mushroom_SK'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -55.0f));  // Ä¸½¶ ÄÄÆ÷³ÍÆ®ÀÇ Àý¹Ý ³ôÀÌ¿¡ ¸ÂÃã
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // ÇÊ¿ä ½Ã È¸Àü Á¶Á¤

	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/AB_RP_Mushroom.AB_RP_Mushroom_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EnemyDeadMontageRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/MushroomDead.MushroomDead"));
	if (EnemyDeadMontageRef.Object)
	{
		EnemyDeadMontage = EnemyDeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EnemyAttackMontageRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/MushroomAttack.MushroomAttack"));
	if (EnemyAttackMontageRef.Object)
	{
		AttackActionMontage = EnemyAttackMontageRef.Object;
	}

	ExperienceValue = 100.0f;
}

void AMushroomCharacter::SetDead()
{
	Super::SetDead();
}

void AMushroomCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(EnemyDeadMontage, 1.0f);
}

void AMushroomCharacter::NotifyAttackActionEnd()
{
	Super::NotifyAttackActionEnd();

	OnAttackFinished.ExecuteIfBound();
}
