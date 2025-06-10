// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSCharactor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/TPSAnimInstance.h"

// Sets default values
ATPSCharactor::ATPSCharactor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	// Mesh
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryCharDark/MW_Style2_Female.MW_Style2_Female'"));

	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Character.ABP_Character_C'"));

	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimInstanceRef.Class);
	}

	// SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 50.0f, 50.0f);

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

#pragma region InputSystem
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCDefaultRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));

	if (IMCDefaultRef.Succeeded())
	{
		IMCDefault = IMCDefaultRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TurnActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Turn.IA_Turn'"));
	if (TurnActionRef.Succeeded())
	{
		TurnAction = TurnActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (RunActionRef.Succeeded())
	{
		RunAction = RunActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Fire.IA_Fire'"));
	if (FireActionRef.Succeeded())
	{
		FireAction = FireActionRef.Object;
	}
#pragma endregion

}


// Called when the game starts or when spawned
void ATPSCharactor::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (SubSystem)
		{
			SubSystem->AddMappingContext(IMCDefault, 0);
		}
	}
}

// Called every frame
void ATPSCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharactor::Input_Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATPSCharactor::Input_Turn);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ATPSCharactor::Input_Run);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATPSCharactor::Input_Fire);
	}

}

void ATPSCharactor::Input_Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void ATPSCharactor::Input_Turn(const FInputActionValue& InputValue)
{
	const FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATPSCharactor::Input_Run(const FInputActionValue& InputValue)
{
	const bool isRun = InputValue.Get<bool>();

	if (isRun == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void ATPSCharactor::Input_Fire(const FInputActionValue& InputValue)
{
	UTPSAnimInstance* AnimInstace = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstace)
	{
		AnimInstace->PlayFireMontage();
	}
}

