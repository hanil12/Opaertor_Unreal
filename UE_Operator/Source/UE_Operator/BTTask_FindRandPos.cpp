// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandPos.h"
#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandPos::UBTTask_FindRandPos()
{
	NodeName = TEXT("FindRandPos");
}

EBTNodeResult::Type UBTTask_FindRandPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(currentPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* navySystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if(navySystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation randLoction;

	if(navySystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, randLoction))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandPos")), randLoction.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
