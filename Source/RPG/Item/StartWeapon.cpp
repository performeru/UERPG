// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/StartWeapon.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerCharacter.h"

// Sets default values
AStartWeapon::AStartWeapon()
{
	// SkeletalMeshComponent 생성, 루트 컴포넌트로 설정
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// 충돌 영역 (Sphere) 생성 및 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(100.0f); // 범위 설정
	CollisionComponent->SetupAttachment(RootComponent);

	// 오버랩 이벤트 활성화
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
		// AStartWeapon을 캐릭터가 직접 장착하도록 수정
		PlayerCharacter->EquipWeapon(this); // this는 AStartWeapon* 타입
		Destroy();  // 무기 제거 (혹은 숨기기)
	}
}

