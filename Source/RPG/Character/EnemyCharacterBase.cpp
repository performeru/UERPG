// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"
#include "EnemyCharacter/RP_EnemyAttack.h"


// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 55.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RPCapsule"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<URP_EnemyAttack> AttackActionDataRef(TEXT("/Game/EnemyCharacterAction/AttackActionData.AttackActionData"));
	if (AttackActionDataRef.Succeeded())
	{
		AttackActionData = AttackActionDataRef.Object;
	}

}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SetDead();

	return DamageAmount;
}

void AEnemyCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
	PlayDeadAnimation();
}

void AEnemyCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstanc = GetMesh()->GetAnimInstance();
	AnimInstanc->StopAllMontages(0.0f);
	AnimInstanc->Montage_Play(EnemyDeadMontage, 3.0f);
}

void AEnemyCharacterBase::ProcessAttackAction()
{
	if (CurrentAttack == 0)
	{
		AttackActionBegin();
		return;
	}

}

void AEnemyCharacterBase::AttackActionBegin()
{
	CurrentAttack = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackActionMontage)
	{
		AnimInstance->Montage_Play(AttackActionMontage, AttackSpeedRate);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AEnemyCharacterBase::AttackActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackActionMontage);

		SetAttackCheckTimer();
	}
}

void AEnemyCharacterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentAttack != 0);
	CurrentAttack = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyAttackActionEnd();
}

void AEnemyCharacterBase::NotifyAttackActionEnd(){}

void AEnemyCharacterBase::SetAttackCheckTimer()
{
	int32 ComboIndex = CurrentAttack - 1;

	AttackActionData->EffectiveFrameCount.IsValidIndex(ComboIndex);
	
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (AttackActionData->EffectiveFrameCount[ComboIndex] / AttackActionData->FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacterBase::AttackCheck, ComboEffectiveTime, false);
	}
	
}

void AEnemyCharacterBase::AttackCheck()
{
	AttackTimerHandle.Invalidate();
	if (HasNextAttackAction)
	{
		UAnimInstance* AnimInstanc = GetMesh()->GetAnimInstance();

		CurrentAttack = FMath::Clamp(CurrentAttack + 1, 1, AttackActionData->MaxAttackCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackActionData->MontageSectionNamePrefix, CurrentAttack);
		AnimInstanc->Montage_JumpToSection(NextSection, AttackActionMontage);
		SetAttackCheckTimer();
		HasNextAttackAction = false;
	}
}

float AEnemyCharacterBase::GetAIPatrolRadius()
{
	return 500.0f;
}

float AEnemyCharacterBase::GetAIDetectRange()
{
	return 800.0f;
}

float AEnemyCharacterBase::GetAIAttackRange()
{
	return EnemyAttackRadius + EnemyAttackRange * 2;
}

float AEnemyCharacterBase::GetAITurnSpeed()
{
	return 2.0f;
}

void AEnemyCharacterBase::AttackByAI()
{
	ProcessAttackAction();
}

void AEnemyCharacterBase::SetAIAttackDelegate(const FAICharacterEnemyAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

