// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMath.h"
#include "MyAnimInstance.h"
#include "MyHpWidget.h"
#include "StatComponent.h"
#include "Components/WidgetComponent.h"

// Log ��� ��
// UE_LOG(LogTemp, Log, TEXT("Pitch Value : %f"), value);
// LogCategory�� ���� ���� ���� �� �ִ�.

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region SpringArm&Camera
	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;

	_springArm->bUsePawnControlRotation = true;
	_camera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
#pragma endregion

#pragma region SetMesh&Character
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));

	if (SM.Succeeded())
		GetMesh()->SetSkeletalMesh(SM.Object);

	// Character Rotation Setting
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate.Yaw = 240;
#pragma endregion 

#pragma region AnimationInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("AnimBlueprint'/Game/BluePrint/MyAnimInstance_BP.MyAnimInstance_BP_C'"));

	if (AnimInstance.Succeeded())
		GetMesh()->AnimClass = AnimInstance.Class;
#pragma endregion

	_statComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));
	_hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	_hpBarWidget->SetupAttachment(GetMesh());
	_hpBarWidget->SetRelativeLocation(FVector(0.0f,0.0f,230.0f));
	_hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/BP_UI.BP_UI_C'"));
	if(UW.Succeeded())
	{
		_hpBarWidget->SetWidgetClass(UW.Class);
		_hpBarWidget->SetDrawSize(FVector2D(200.0f,50.0f));
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_animInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(_animInstance))
	{
		_animInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		_animInstance->_onAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	}

	_hpBarWidget->InitWidget();
	auto hpWidget = Cast<UMyHpWidget>(_hpBarWidget->GetUserWidgetObject());
	if(hpWidget)
	{
		hpWidget->BindHp(_statComponent);
	}
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
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);
}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	_statComponent->Damaged(Damage);

	return Damage;
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

void AMyCharacter::Pitch(float value)
{
	AddControllerPitchInput(value);
}

void AMyCharacter::Attack()
{
	if(_isAttacking)
		return;

	GetCharacterMovement()->RotationRate.Yaw = 360;
	_animInstance->PlayerAttackMontage();
	_animInstance->JumpToSection(_attackIndex);

	_attackIndex = (_attackIndex + 1) % 3;

	_isAttacking = true;
}

void AMyCharacter::AttackCheck()
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

	// 캡슐 충돌
	// bool boolResult = GetWorld()->SweepSingleByChannel(
	// OUT hitResult,
	// GetActorLocation(),
	// GetActorLocation() + GetActorForwardVector() * attackRange,
	// FQuat::Identity,
	// ECollisionChannel::ECC_GameTraceChannel2,
	// FCollisionShape::MakeSphere(attackRadius),
	// params);

	FColor DrawColor;
	if (boolResult)
	{
		DrawColor = FColor::Red;
	}
	else
	{
		DrawColor = FColor::Green;
	}
	
	
	// 캡슐 충돌 DrawDebug
	// FVector vec = GetActorForwardVector() * attackRange;
	// FVector center = GetActorLocation() + vec * 0.5f;
	// float halfHeight = attackRange * 0.5f + attackRadius;
	// FQuat rotation = FRotationMatrix::MakeFromZ(vec).ToQuat();
	// DrawDebugCapsule(GetWorld(), center, halfHeight, attackRadius, rotation, DrawColor, false, 2.0f);
	
	DrawDebugLine(GetWorld(), hitScanStart, hitScanEnd,DrawColor, false, 2.0f);

	if(boolResult && hitResult.Actor.IsValid())
	{
		FDamageEvent damageEvent;
		hitResult.Actor->TakeDamage(_statComponent->GetAtk(), damageEvent, GetController(), this);
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	_isAttacking = false;
	GetCharacterMovement()->RotationRate.Yaw = 240;
}

