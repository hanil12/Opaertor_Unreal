// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMath.h"

// Log 찍는 법
// UE_LOG(LogTemp, Log, TEXT("Pitch Value : %f"), value);
// LogCategory도 내가 직접 만들 수 있다.

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f,0.0f,0.0f));

	_springArm->bUsePawnControlRotation = true;
	_camera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));

	if (SM.Succeeded())
		GetMesh()->SetSkeletalMesh(SM.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("AnimBlueprint'/Game/BluePrint/MyAnimInstance_BP.MyAnimInstance_BP_C'"));

	if (AnimInstance.Succeeded())
		GetMesh()->AnimClass = AnimInstance.Class;

	// Character Rotation Setting
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate.Yaw = 120;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);
}

void AMyCharacter::UpDown(float value)
{
	_upDown = value;
	auto controllerRotator = GetControlRotation();
	auto controllerVector = controllerRotator.Vector();

	AddMovementInput(controllerVector,value);
}

void AMyCharacter::LeftRight(float value)
{
	_leftRight = value;
	auto controllerRotator = GetControlRotation();
	auto controllerVector = controllerRotator.Vector();
	controllerVector = controllerVector.RotateAngleAxis(90, FVector(0.0f,0.0f,1.0f));

	AddMovementInput(controllerVector, value);
}

void AMyCharacter::Yaw(float value)
{
	AddControllerYawInput(value);

	float degree = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw,GetControlRotation().Yaw);

	if (degree > 90.0f)
	{
		_isTurnRight = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if(degree < - 90.0f)
	{
		_isTurnLeft = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if(FMath::Abs(degree) < 0.1f)
	{
		_isTurnLeft = false;
		_isTurnRight = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

}

void AMyCharacter::Pitch(float value)
{
	AddControllerPitchInput(value);
}

