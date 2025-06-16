// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(6.0f);
	SphereCollision->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/_Art/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_545x39.SM_Shell_545x39'"));

	if (MeshRef.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshRef.Object);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComponent->SetUpdatedComponent(GetRootComponent());
	MovementComponent->InitialSpeed = InitSpeed;
	MovementComponent->MaxSpeed = MaxSpeed;
	MovementComponent->bRotationFollowsVelocity = false;
	MovementComponent->bShouldBounce = false;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Fire(const FVector& Direction) const
{
	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
}

