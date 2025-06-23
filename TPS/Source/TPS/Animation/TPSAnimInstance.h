// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTPSAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayFireMontage();
	void PlayReloadMontage();

	UFUNCTION()
	void FinishReloading(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TObjectPtr<class UAnimMontage> ReloadMontage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ATPSCharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BluePrintReadOnly)
	float ForwardSpeed = -0.0f;

	UPROPERTY(BlueprintReadOnly)
	float RightSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;

};
