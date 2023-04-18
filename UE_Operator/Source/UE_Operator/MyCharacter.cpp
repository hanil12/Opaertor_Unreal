// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "DrawDebugHelpers.h"
#include "MyAIController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMath.h"
#include "MyAnimInstance.h"
#include "MyGameInstance.h"
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
		_animInstance->_onDeathEnd.AddUObject(this, &AMyCharacter::Died);
		_animInstance->_onDeathStart.AddUObject(this, &AMyCharacter::DiedStart);
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
}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	_statComponent->Damaged(Damage);

	return Damage;
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

void AMyCharacter::Died()
{
	SetActorHiddenInGame(true);
	
	for(auto component : GetComponents())
	{
		component->Deactivate();
	}
}

void AMyCharacter::DiedStart()
{
	Cast<UMyGameInstance>(GetGameInstance())->PlayEffect("Death",GetActorLocation());
	GetController()->UnPossess();
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	_isAttacking = false;
	GetCharacterMovement()->RotationRate.Yaw = 240;
	_attackEnd.Broadcast();
}

