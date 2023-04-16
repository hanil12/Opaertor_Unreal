// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyCharacter.h"

#include "DrawDebugHelpers.h"
#include "MyAIController.h"
#include "MyAnimInstance.h"
#include "StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyEnemyCharacter::AMyEnemyCharacter()
{
	SetMesh();

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(IsValid(_animInstance))
		_animInstance->_onAttackHit.AddUObject(this, &AMyEnemyCharacter::AttackCheck);
}

void AMyEnemyCharacter::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 500.0f;
	float attackRadius = 50.0f;

	FVector hitScanStart = GetActorLocation();
	FVector hitScanEnd = GetActorLocation() + GetActorForwardVector() * attackRange;
	
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
	}
}

void AMyEnemyCharacter::SetMesh()
{
	GetMesh()->SetRelativeLocationAndRotation(
	FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));

	if (SM.Succeeded())
		GetMesh()->SetSkeletalMesh(SM.Object);

	// Character Rotation Setting
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate.Yaw = 360;
}
