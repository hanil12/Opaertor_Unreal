// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_MyCharacter.BT_MyCharacter'"));

	if(BT.Succeeded())
	{
		_behaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_MyCharacter.BB_MyCharacter'"));

	if(BD.Succeeded())
	{
		_blackBoardData = BD.Object;
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	//GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AMyAIController::RandMove, 3.0f, true);

	if(UseBlackboard(_blackBoardData, Blackboard))
	{
		if(RunBehaviorTree(_behaviorTree))
		{
			UE_LOG(LogTemp, Log, TEXT("Behavior Succeed"));
		}
	}
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
}

void AMyAIController::RandMove()
{
	auto currentPawn = GetPawn();

	UNavigationSystemV1* navySystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if(navySystem == nullptr)
		return;

	FNavLocation randLoction;

	if(navySystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, randLoction))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, randLoction);
	}
}
