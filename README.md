## 1. 구현 동영상
### Youtube
[![구현 동영상](http://img.youtube.com/vi/_RFcu8tAdac/0.jpg)](https://www.youtube.com/watch?v=_RFcu8tAdac)
___

## 2. 플레이어 캐릭터 & 애니메이션
<img src="https://github.com/user-attachments/assets/43efc0c6-0f79-4026-8b7a-dace6ed6bc2d" width="400"/>  

```cpp
APlayerCharacter::APlayerCharacter()
{
	// 체력 상태 컴포넌트 초기화
	Stat = CreateDefaultSubobject<URPCharacterStatComponent>(TEXT("StatComponent"));
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

float URPCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0 ,InDamage);

	SetHp(PrevHp - ActualDamage);
	if(CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}
```



<img src="https://github.com/user-attachments/assets/a31009bb-bc0e-479e-ba8f-542b606fdc2e" width="600"/> 



### 관련 코드: [캐릭터 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/Character)

## 3. 몬스터 & 애니메이션

<img src="https://github.com/user-attachments/assets/75b43445-49b1-4142-8d05-52445526bd89" width="400"/> 
<img src="https://github.com/user-attachments/assets/7e4d6610-5c93-4c52-a6b7-3f2e13a42794" width="400"/> 

```cpp
float URPEnemyStatComponent::ApplyDamage(float InDamage)
{
    const float PrevHp = EnemyCurrentHp;
    const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
    EnemySetHp(PrevHp - ActualDamage);

    if(EnemyCurrentHp <= KINDA_SMALL_NUMBER)
    {
	    OnEnemyHpZero.Broadcast();
    }
    return ActualDamage;
}

void AEnemySpawnManager::SpawnEnemy()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AEnemyCharacterBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedEnemy)
		{
			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AEnemySpawnManager::OnEnemyDeath);
		}
	}
	
}

void AEnemySpawnManager::OnEnemyDeath(AActor* DestroyedActor)
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnManager::SpawnEnemy, SpawnDelay, false);
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
```

<img src="https://github.com/user-attachments/assets/4a8d4ca5-e58c-46c3-91a2-e8368075e9de" width="600"/> 

### 관련 코드: [몬스터 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/EnemyCharacter)

## 4. AI 
### 비헤이어 트리

<img src="https://github.com/user-attachments/assets/9b85553d-72a2-4b2d-b962-62cdbb58a030" width="600"/> 

```cpp 
void AEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if(UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
	}
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IRPAIAllInterface* AIPawn = Cast<IRPAIAllInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
```

### 관련 코드: [AI 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/AI)

___

## 느끼점
언리얼을 처음 공부하고 구현을 해보려고 했을 때, 에셋을 구하는 것도 너무 어려웠다. 공부를 하면서 구현을 진행했고, 특히 애니메이션에서 몬스터의 죽음 애니메이션을 구현하는 게 어려웠다. 단순 구현만 하는 건 괜찮았는데, 공격 애니메이션과 연결했을 때 몬스터가 죽었을 시 죽음 애니메이션이 실행되어야 하는데, 계속 공격 애니메이션이 실행되는 바람에 해결하는 데 어려움을 겪었다. 해결법을 생각하던 중 bIsDead 변수를 만들어 이미 죽은 상태라면 추가 처리를 하지 않도록 하였고, 애니메이션 인스턴스에서 Montage_Play()를 통해 죽음 애니메이션을 재생할 때 StopAllMontages(0.0f)를 호출하여 모든 애니메이션을 멈추고 새로운 애니메이션을 시작하게 해서 문제를 해결했다.

구현할수록 코드가 길어지면서 주석의 중요성도 알게 되었다. 처음 구현을 시작하면서 주석을 쓰지 않고 로직을 만들고 구현하다 보니, 나중에는 왜 그렇게 구현했는지 알 수 없게 되어 뒤늦게 주석을 달기 시작했다.

설계 부분에서도 섹션을 지정해서 만드는 것이 좋겠다고 느꼈다. 섹션 없이 만들다 보니 주석이 없는 것처럼 어느 부분인지 헷갈리게 되었고 앞으로는 주석을 쓰는 습관을 들이고, 섹션도 만들어 설계를 더 깔끔하게 할 수 있도록 해야겠다.
