// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/StartWeapon.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerCharacter.h"

// Sets default values
AStartWeapon::AStartWeapon()
{
	// SkeletalMeshComponent ����, ��Ʈ ������Ʈ�� ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// �浹 ���� (Sphere) ���� �� ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(100.0f); // ���� ����
	CollisionComponent->SetupAttachment(RootComponent);

	// ������ �̺�Ʈ Ȱ��ȭ
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AStartWeapon::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StartWeapon(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_SwordB/SK_Blade_SwordB.SK_Blade_SwordB"));
	if (StartWeapon.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(StartWeapon.Object);
	}
}

// Called when the game starts or when spawned
void AStartWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// AStartWeapon�� ĳ���Ͱ� ���� �����ϵ��� ����
		PlayerCharacter->EquipWeapon(this); // this�� AStartWeapon* Ÿ��
		Destroy();  // ���� ���� (Ȥ�� �����)
	}
}

