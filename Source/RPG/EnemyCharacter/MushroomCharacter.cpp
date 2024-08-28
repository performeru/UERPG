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
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -55.0f));  // 캡슐 컴포넌트의 절반 높이에 맞춤
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 필요 시 회전 조정

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

	// 컴포넌트가 정상적으로 생성되었다면 초기화
	if (EnemyStat)
	{
		// Setter 함수를 이용해 초기화
		EnemyStat->SetEnemyHp(100.0f);           // 최대 체력 설정
		EnemyStat->SetEnemyCurrentHp(100.0f);    // 현재 체력 설정
		EnemyStat->SetEnemyLevel(1.0f);           // 레벨 초기화
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
		// 공격자의 공격력을 데미지에 반영
		float AttackPower = Attacker->GetStat()->GetAttackPower();
		UE_LOG(LogTemp, Warning, TEXT("Player Attack Power: %f"), AttackPower);

		DamageAmount += AttackPower; // 공격력 추가
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage Amount before ApplyDamage: %f"), DamageAmount);

	return EnemyStat->ApplyDamage(DamageAmount);
}
