// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "MyAnimInstance.h"
#include "MyGameInstance.h"
#include "StatComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPlayerCharacter::AMyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	
	SetUpSpringArmAndCamera();
	SetMesh();
}

void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyPlayerCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPlayerCharacter::Pitch);
}

void AMyPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(IsValid(_animInstance))
		_animInstance->_onAttackHit.AddUObject(this, &AMyPlayerCharacter::AttackCheck);

	_statComponent->SetStatByLevel(5);
	_statComponent->SetFullHp();
}

void AMyPlayerCharacter::SetMesh()
{
	GetMesh()->SetRelativeLocationAndRotation(
	FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));

	if (SM.Succeeded())
		GetMesh()->SetSkeletalMesh(SM.Object);

	// Character Rotation Setting
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate.Yaw = 240;
}

void AMyPlayerCharacter::SetUpSpringArmAndCamera()
{
	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 400.0f;

	_springArm->bUsePawnControlRotation = true;
	_camera->bUsePawnControlRotation = false;
}

void AMyPlayerCharacter::UpDown(float value)
{
	_upDown = value;
	auto controllerRotator = GetControlRotation();
	auto controllerVector = controllerRotator.Vector();

	AddMovementInput(controllerVector,value);
}

void AMyPlayerCharacter::LeftRight(float value)
{
	_leftRight = value;
	auto controllerRotator = GetControlRotation();
	auto controllerVector = controllerRotator.Vector();
	controllerVector = controllerVector.RotateAngleAxis(90, FVector(0.0f,0.0f,1.0f));

	AddMovementInput(controllerVector, value);
}

void AMyPlayerCharacter::Yaw(float value)
{
	AddControllerYawInput(value);

	float degree = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw,GetControlRotation().Yaw);

	if (degree > 90.0f)
	{
		_isTurnRight = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else if(degree < - 90.0f)
	{
		_isTurnLeft = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else if (GetCharacterMovement()->Velocity.Size() > 0.01f || _isAttacking)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else if(FMath::Abs(degree) < 0.1f)
	{
		_isTurnLeft = false;
		_isTurnRight = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void AMyPlayerCharacter::Pitch(float value)
{
	AddControllerPitchInput(value);
}

void AMyPlayerCharacter::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 1900.0f;
	float attackRadius = 50.0f;

	FVector hitScanStart = _camera->GetComponentLocation();
	FVector hitScanEnd = _camera->GetComponentLocation() + _camera->GetForwardVector() * attackRange;
	
	bool boolResult = GetWorld()->LineTraceSingleByChannel(
		OUT hitResult,
		hitScanStart,
		hitScanEnd,
		ECollisionChannel::ECC_GameTraceChannel2,
		params);

	FColor DrawColor;
	if (boolResult)
	{
		DrawColor = FColor::Red;
	}
	else
	{
		DrawColor = FColor::Green;
	}

	DrawDebugLine(GetWorld(), hitScanStart, hitScanEnd,DrawColor, false, 2.0f);

	if(boolResult && hitResult.Actor.IsValid())
	{
		FDamageEvent damageEvent;
		hitResult.Actor->TakeDamage(_statComponent->GetAtk(), damageEvent, GetController(), this);
		Cast<UMyGameInstance>(GetGameInstance())->PlayEffect("AttackHit",hitResult.Location);
	}
}

