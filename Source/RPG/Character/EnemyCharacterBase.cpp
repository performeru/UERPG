// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"


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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EnemyDeadMontageRef(TEXT("/Game/MonsterForSurvivalGame/Animation/PBR/Mushroom/MushroomDead.MushroomDead"));
	if (EnemyDeadMontageRef.Object)
	{
		EnemyDeadMontage = EnemyDeadMontageRef.Object;
	}

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
	PlayDeadAnimation();
	SetActorEnableCollision(false);

}

void AEnemyCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstanc = GetMesh()->GetAnimInstance();
	AnimInstanc->StopAllMontages(0.0f);
	AnimInstanc->Montage_Play(EnemyDeadMontage, 1.0f);
}

float AEnemyCharacterBase::GetAIPatrolRadius()
{
	return 500.0f;
}

float AEnemyCharacterBase::GetAIDetectRange()
{
	return 400.0f;
}

float AEnemyCharacterBase::GetAIAttackRange()
{
	return 0.0f;
}

float AEnemyCharacterBase::GetAITurnSpeed()
{
	return 0.0f;
}

