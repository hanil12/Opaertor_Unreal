// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MyWait.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "MyPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_MyWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(SearchTarget(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
}

bool UBTTask_MyWait::SearchTarget(UBehaviorTreeComponent& OwnerComp)
{
	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	UWorld* world = currentPawn->GetWorld();

	if(currentPawn == nullptr)
		return false;

	if(world == nullptr)
		return false;

	FVector center = currentPawn->GetActorLocation();

	float SearchRadius =  500.0f;

	TArray<FOverlapResult> overlapResult;
	FCollisionQueryParams param(NAME_None, false, currentPawn);
	
	
	bool bResult = world->OverlapMultiByChannel(
		overlapResult,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		param
		);

	if(bResult)
	{
		for(auto& result : overlapResult)
		{
			AMyPlayerCharacter* player = Cast<AMyPlayerCharacter>(result.GetActor());
			if(player && player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), player);
				
				return true;
			}
		}
	}

	return false;
}
