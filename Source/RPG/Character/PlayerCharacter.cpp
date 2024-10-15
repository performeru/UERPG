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
#include "UI/ExperienceWidget.h" 
#include "Item/WeaponItemData.h"
#include "Item/StartWeapon.h"
#include "Components/SkeletalMeshComponent.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
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

	// 입력
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


	// Item Action
	TakeItemAction.Add(FTakeItemDelegate(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemAction.Add(FTakeItemDelegate(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::DrinkPotion)));

	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	// Level
	PlayerLevel = 1;
	CurrentExperience = 0.0f;
	ExperienceToNextLevel = 100.0f;

	// Set default values for the ExperienceWidgetClass
	static ConstructorHelpers::FClassFinder<UExperienceWidget> WidgetClassFinder(TEXT("/Game/UI/ExperienceWidget.ExperienceWidget_C"));
	if (WidgetClassFinder.Succeeded())
	{
		ExperienceWidgetClass = WidgetClassFinder.Class;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	SetCharacterControl(CurrentCharacterControlType);

	Stat->OnHpChanged.AddUObject(this, &APlayerCharacter::UpdateHealthUI);

	// 경험치 위젯을 생성하고 화면에 추가
	if (ExperienceWidgetClass)
	{
		ExperienceWidget = CreateWidget<UExperienceWidget>(GetWorld(), ExperienceWidgetClass);
		if (ExperienceWidget)
		{
			ExperienceWidget->AddToViewport();
			UpdateExperienceUI();  // 초기 경험치 값을 반영
		}
	}
}

// Dead
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &APlayerCharacter::SetDead);
}

// View
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	const float Sensitivity = 0.03f;

	AddControllerYawInput(LookAxisVector.X * Sensitivity);
	AddControllerPitchInput(LookAxisVector.Y * Sensitivity);
}

void APlayerCharacter::QuaterMove(const FInputActionValue& Value)
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

void APlayerCharacter::Attack()
{
	ProcessComboCommand();

	UE_LOG(LogTemp, Warning, TEXT("Player Attack Power: %f"), Stat->GetAttackPower());
}

void APlayerCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 0.0f;
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
		CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
		CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
		CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
		CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
		CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
		CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
		CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
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

void APlayerCharacter::TakeItem(UItemDataAsset* InItemData)
{
	if(InItemData)
	{
		TakeItemAction[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void APlayerCharacter::DrinkPotion(UItemDataAsset* InItemData)
{
	if (InItemData)
	{
		if (InItemData->Type == EItem::Potion)
		{
			// 포션의 회복량
			float RestoreAmount = InItemData->HpAmount;

			// 현재 HP와 최대 HP를 가져와서 회복량 적용
			float NewHp = Stat->GetCurrentHp() + RestoreAmount;
			NewHp = FMath::Clamp(NewHp, 0.0f, Stat->GetMaxHp());

			// HP를 설정
			Stat->UpdateHp(NewHp);
		}
	}
}

void APlayerCharacter::EquipWeapon(UItemDataAsset* InItemData)
{
	UWeaponItemData* WeaponItemAsset = Cast<UWeaponItemData>(InItemData);

	if(InItemData)
	{
		if(WeaponItemAsset->WeaponMesh.IsPending())
		{
			WeaponItemAsset->WeaponMesh.LoadSynchronous();
		}
		this->Weapon->SetSkeletalMesh(WeaponItemAsset->WeaponMesh.Get());
	}
}
void APlayerCharacter::EquipWeapon(AStartWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		USkeletalMeshComponent* NewWeaponMesh = NewWeapon->GetWeaponMesh();

		if (NewWeaponMesh)
		{
			this->Weapon->SetSkeletalMesh(NewWeaponMesh->GetSkeletalMeshAsset()); 
		}
	}
}
// UI
void APlayerCharacter::UpdateExperienceUI()
{
	if (ExperienceWidget)
	{
		ExperienceWidget->UpdateExperience(CurrentExperience, ExperienceToNextLevel);
		ExperienceWidget->SetLevel(PlayerLevel); // 레벨 UI 업데이트
	}
}

void APlayerCharacter::GainExperience(float Amount)
{
	CurrentExperience += Amount;

	if (CurrentExperience >= ExperienceToNextLevel)
	{
		LevelUp();
	}
	UpdateExperienceUI();  // UI 업데이트
}

void APlayerCharacter::LevelUp()
{
	PlayerLevel++;
	CurrentExperience = CurrentExperience - ExperienceToNextLevel;
	ExperienceToNextLevel *= 1.5f;  // 레벨업할 때마다 필요한 경험치 증가

	if (Stat)
	{
		Stat->LevelUpStats(); // 스탯 증가 함수 호출
	}

	UpdateExperienceUI();
	UpdateHealthUI(Stat->GetCurrentHp()); // 체력 UI 업데이트
}

