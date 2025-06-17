// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

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
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshComponent->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));
	MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/_Art/Effect/P_HitEffect.P_HitEffect'"));
	if (HitEffectRef.Succeeded())
	{
		HitEffect = HitEffectRef.Object;
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
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

void ABullet::PlayHitEffect(FTransform HitTransform)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitTransform);
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::Printf(TEXT("Bullet Hit")));

	FTransform BulletTransform;
	BulletTransform.SetLocation(Hit.ImpactPoint);
	PlayHitEffect(BulletTransform);

	Destroy();
}

