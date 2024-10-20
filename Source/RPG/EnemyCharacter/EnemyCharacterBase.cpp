// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"
#include "Engine/DamageEvents.h"
#include "EnemyCharacter/RP_EnemyAttack.h"
#include "CharacterStat/RPEnemyStatComponent.h"
#include "Character/PlayerCharacter.h"
#include "UI/RPWidgetComponent.h"
#include "UI/EnemyHpBarWidget.h"
#include "Item/ItemBox.h"


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

	// Stat Component 
	EnemyStat = CreateDefaultSubobject<URPEnemyStatComponent>(TEXT("EnemyStat"));

	// Widget Component 
	EnemyHpBar = CreateDefaultSubobject<URPWidgetComponent>(TEXT("EnemyWidget"));
	EnemyHpBar->SetupAttachment(GetMesh());
	EnemyHpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/EnemyHpBar.EnemyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		EnemyHpBar->SetWidgetClass(HpBarWidgetRef.Class);
		EnemyHpBar->SetWidgetSpace(EWidgetSpace::Screen);
		EnemyHpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		EnemyHpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bIsDead = false;
}

void AEnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyStat->OnEnemyHpZero.AddUObject(this, &AEnemyCharacterBase::SetDead);
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

	return DamageAmount;
}

void AEnemyCharacterBase::SetDead()
{
	if (bIsDead)
	{
		return; // 이미 죽은 상태라면 추가 처리를 하지 않음
	}

	bIsDead = true;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	// 아이템 박스 생성
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, -60.0f); // 적의 위치 위에 아이템 박스를 스폰
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AItemBox>(AItemBox::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);


	// 플레이어에게 경험치 제공
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->GainExperience(ExperienceValue);  // 적 처치 시 제공할 경험치 값
	}

	FTimerHandle DeadTimerHanlde;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHanlde, FTimerDelegate::CreateLambda([&]() { Destroy(); }), EnemyDeadEventDelayTime, false);
}

void AEnemyCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (AnimInstance->Montage_IsPlaying(AttackActionMontage))
		{
			AnimInstance->Montage_Stop(0.1f, AttackActionMontage);  // 공격 몽타주 중지
		}
		AnimInstance->Montage_Play(EnemyDeadMontage, 1.0f);  // 데드 몽타주 재생
	}
}

void AEnemyCharacterBase::ProcessAttackAction()
{
	if (bIsDead)
	{
		return;  // 죽은 상태라면 공격을 무시함
	}

	if (CurrentAttack == 0)
	{
		AttackActionBegin();
		return;
	}
}

void AEnemyCharacterBase::AttackActionBegin()
{
	if (bIsDead)
	{
		return;  // 죽은 상태라면 공격 시작을 무시
	}

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
	if (bIsDead)
	{
		return;  // 죽은 상태라면 공격 체크를 중단함
	}

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

void AEnemyCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float EnemyAttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * EnemyAttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(EnemyAttackDamage, DamageEvent, GetController(), this);
	}
}

void AEnemyCharacterBase::SetupEnemyWidget(URPUserWidget* InUserWidget)
{
	UEnemyHpBarWidget* EnemyHpBarWidget = Cast<UEnemyHpBarWidget>(InUserWidget);
	if(EnemyHpBarWidget)
	{
		float MaxHp = EnemyStat->EnemyGetHp();
		float CurrentHp = EnemyStat->EnemyGetCurrentHp();

		EnemyHpBarWidget->EnemySetMaxHp(MaxHp);
		EnemyHpBarWidget->UpdateEnemyHpBar(CurrentHp);

		EnemyStat->OnEnemyHpChanged.AddUObject(EnemyHpBarWidget, &UEnemyHpBarWidget::UpdateEnemyHpBar);
	}

}

