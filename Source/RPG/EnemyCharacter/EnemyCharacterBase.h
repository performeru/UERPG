// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RPAIAllInterface.h"
#include "Interface/RPAnimationAttackInterface.h"
#include "Interface/RPEnemyWidgetInterface.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class RPG_API AEnemyCharacterBase : public ACharacter, public IRPAIAllInterface, public IRPAnimationAttackInterface, public IRPEnemyWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> EnemyDeadMontage;

	virtual void SetDead();
	virtual void PlayDeadAnimation();

	float EnemyDeadEventDelayTime = 3.0f;
	
	// Attack 새로추가한거 보고 삭제
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowAbstract = "true"))
	TObjectPtr<class URP_EnemyAttack> AttackActionData;

	void ProcessAttackAction();
	void AttackActionBegin();
	void AttackActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyAttackActionEnd();
	void SetAttackCheckTimer();
	void AttackCheck();

	int32 CurrentAttack = 0;
	FTimerHandle AttackTimerHandle;
	bool HasNextAttackAction = false;
	
protected:
	// Ai 
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAICharacterEnemyAttackFinished& InOnAttackFinished) override;
	
	FAICharacterEnemyAttackFinished OnAttackFinished;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float EnemyLevel;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float EnemyHp;

	const float EnemyAttackRange = 40.0f;
	const float EnemyAttackRadius = 50.0f;

	// Attack hit
protected:
	virtual void AttackHitCheck() override;

protected:
	// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URPEnemyStatComponent> EnemyStat;
	// UI Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URPWidgetComponent> EnemyHpBar;

	virtual void SetupEnemyWidget(class URPUserWidget* InUserWidget) override;
// 경험치
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ExperienceValue;  // 적 처치 시 제공하는 경험치 값

private:
	bool bIsDead;
};
