// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/MushroomCharacter.h"
#include "Character/PlayerCharacter.h"
#include "CharacterStat/RPEnemyStatComponent.h"


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

	// ������Ʈ�� ���������� �����Ǿ��ٸ� �ʱ�ȭ
	if (EnemyStat)
	{
		// Setter �Լ��� �̿��� �ʱ�ȭ
		EnemyStat->SetEnemyHp(100.0f);           // �ִ� ü�� ����
		EnemyStat->SetEnemyCurrentHp(100.0f);    // ���� ü�� ����
		EnemyStat->SetEnemyLevel(1.0f);           // ���� �ʱ�ȭ
	}
	UE_LOG(LogTemp, Warning, TEXT("Mushroom Character Initialized with HP: %f"), EnemyStat->EnemyGetCurrentHp());

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

float AMushroomCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	APlayerCharacter* Attacker = Cast<APlayerCharacter>(DamageCauser);
	if (Attacker && Attacker->GetStat())
	{
		// �������� ���ݷ��� �������� �ݿ�
		float AttackPower = Attacker->GetStat()->GetAttackPower();
		UE_LOG(LogTemp, Warning, TEXT("Player Attack Power: %f"), AttackPower);

		DamageAmount += AttackPower; // ���ݷ� �߰�
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage Amount before ApplyDamage: %f"), DamageAmount);

	return EnemyStat->ApplyDamage(DamageAmount);
}
