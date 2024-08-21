#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CharacterControlDataAsset.h"
#include "InputAction.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/RPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/RPHUD.h"
#include "UI/CharacterHpInfoWidget.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/RPP_AB_Character.RPP_AB_Character_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>Mapping(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/InputMapping/Mapping.Mapping'"));
	if(Mapping.Object)
	{
		DefaultMappingContext = Mapping.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/ChangeControl.ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCharacterControlDataAsset> ShoulderDataRef(TEXT("/Script/RPG.CharacterControlDataAsset'/Game/CharacterControl/Shoulder.Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCharacterControlDataAsset> QuaterDataRef(TEXT("/Script/RPG.CharacterControlDataAsset'/Game/CharacterControl/Quater.Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/IA_Jump.IA_Jump'"));
	if (IA_Jump.Object)
	{
		JumpAction = IA_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/InputMapping/IA_Attack.IA_Attack'"));
	if (IA_Attack.Object)
	{
		AttackAction = IA_Attack.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;

	// 체력 상태 컴포넌트 초기화
	Stat = CreateDefaultSubobject<URPCharacterStatComponent>(TEXT("StatComponent"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);

	Stat->OnHpChanged.AddUObject(this, &APlayerCharacter::UpdateHealthUI);
}

// Dead
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &APlayerCharacter::SetDead);
}

void APlayerCharacter::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

}

void APlayerCharacter::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
	
}

void APlayerCharacter::QuaterMove(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 카메라의 회전을 가져옵니다.
	const FRotator Rotation = Controller->GetControlRotation();
	UE_LOG(LogTemp, Warning, TEXT("Control Rotation: Pitch=%f, Yaw=%f, Roll=%f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
	UE_LOG(LogTemp, Warning, TEXT("MovementVector: X=%f, Y=%f"), MovementVector.X, MovementVector.Y);
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 회전을 적용하여 이동 벡터를 변환합니다.
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	UE_LOG(LogTemp, Warning, TEXT("ForwardDirection: X=%f, Y=%f, Z=%f"), ForwardDirection.X, ForwardDirection.Y, ForwardDirection.Z);
	UE_LOG(LogTemp, Warning, TEXT("RightDirection: X=%f, Y=%f, Z=%f"), RightDirection.X, RightDirection.Y, RightDirection.Z);

	UE_LOG(LogTemp, Warning, TEXT("Applying movement input: Forward=%f, Right=%f"), MovementVector.X, MovementVector.Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APlayerCharacter::Attack()
{
	ProcessComboCommand();
}

void APlayerCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrignin = Start + (End - Start) * 0.5f;
	float CapsuleHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrignin, CapsuleHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif // ENABLE_DRAW_DEBUG

}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	UpdateHealthUI(Stat->GetCurrentHp());
	
	if (Stat->GetCurrentHp() <= 0.0f)
	{
		SetDead();
	}

	return DamageAmount;
}

void APlayerCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	DisableInput(Cast<APlayerController>(GetController()));

	FTimerHandle DeadTimerHanlde;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHanlde, FTimerDelegate::CreateLambda([&]() { Destroy(); }), DeadEventDelayTime, false);
}

void APlayerCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void APlayerCharacter::UpdateHealthUI(float CurrentHealth)
{
	if(CharacterHpInfoWidget)
	{
		/*float HealthPercent = CurrentHealth / Stat->GetMaxHp();
		CharacterHpInfoWidget->SetHealthBarPercent(HealthPercent);*/
		float HealthPercent = Stat->GetMaxHp() > 0 ? CurrentHealth / Stat->GetMaxHp() : 0.0f;
		CharacterHpInfoWidget->SetHealthBarPercent(HealthPercent);

	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ShoulderMove);
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::QuaterMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ShoulderLook);
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
	}
}

void APlayerCharacter::SetCharacterControlData(const UCharacterControlDataAsset* CharacterControlData)
{
	if(CharacterControlData)
	{
		if (CharacterControlData)
		{
			CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
			CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
			CameraBoom->bUsePawnControlRotation = false; // 캐릭터 회전과 카메라 회전을 분리
			CameraBoom->bInheritPitch = false; // 카메라의 피치(상하) 각도 고정
			CameraBoom->bInheritYaw = false; // 카메라의 요(좌우) 각도 고정
			CameraBoom->bInheritRoll = false;
			CameraBoom->bDoCollisionTest = true; // 카메라가 충돌할 경우의 처리
		}
		
	}

}

void APlayerCharacter::ChangeCharacterControl()
{
	if(CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void APlayerCharacter::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UCharacterControlDataAsset* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl); // 카메라 및 캐릭터 설정 업데이트

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;

}

