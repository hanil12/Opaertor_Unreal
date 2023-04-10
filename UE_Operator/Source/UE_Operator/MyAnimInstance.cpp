// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "MyCharacter.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animMontage(TEXT("AnimMontage'/Game/BluePrint/MyCharacterAnimMontage_BP.MyCharacterAnimMontage_BP'"));

	if (animMontage.Succeeded())
	{
		_attackMontage = animMontage.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* pawn = TryGetPawnOwner();
	if (IsValid(pawn))
	{
		_speed = pawn->GetVelocity().Size();

		AMyCharacter* character = Cast<AMyCharacter>(pawn);

		if (IsValid(character))
		{
			_vertical =		 character->UpDownValue();
			_horizontal =	 character->LeftRightValue();
			_isFalling =	 character->GetMovementComponent()->IsFalling();

			FRotator controllRotation = character->GetControlRotation();
			FRotator actorRotation = character->GetActorRotation();
			_lookLeftRight = FMath::FindDeltaAngleDegrees(actorRotation.Yaw, controllRotation.Yaw);
			_lookUpDown = FMath::FindDeltaAngleDegrees(actorRotation.Pitch, controllRotation.Pitch);

			_turnLeft = character->IsTurnLeft();
			_turnRight = character->IsTurnRight();
		}
	}
}

void UMyAnimInstance::PlayerAttackMontage()
{
	if (!Montage_IsPlaying(_attackMontage))
	{
		Montage_Play(_attackMontage);
	}
}

void UMyAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("PrimaryAttack%d"), sectionIndex));
	Montage_JumpToSection(sectionName, _attackMontage);
}