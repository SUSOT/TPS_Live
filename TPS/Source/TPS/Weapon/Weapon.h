// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE int32 GetAmmoMaxCount() { return AmmoMaxCount; }
	FORCEINLINE int32 GetAmmoRemainCount() { return AmmoRemainCount; }
	FORCEINLINE float GetReloadingDelayTime() { return ReloadingDelayTime; }

	FORCEINLINE void SetAmmoRemainCount(int32 NewAmmoRemainCount)
	{
		AmmoRemainCount = NewAmmoRemainCount;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = properties)
	int32 AmmoMaxCount = 30;

	UPROPERTY(EditAnywhere, Category = properties)
	int32 AmmoRemainCount;

	UPROPERTY(EditAnywhere, Category = properties)
	float ReloadingDelayTime = 3.0f;
};
