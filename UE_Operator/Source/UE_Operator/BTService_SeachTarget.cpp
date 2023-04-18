// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SeachTarget.h"

#include "AIController.h"
#include "MyAIController.h"
#include "MyPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_SeachTarget::UBTService_SeachTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.0f;
}

void UBTService_SeachTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	SearchTarget(OwnerComp);
}

void UBTService_SeachTarget::SearchTarget(UBehaviorTreeComponent& OwnerComp)
{
	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	UWorld* world = currentPawn->GetWorld();

	if(currentPawn == nullptr)
		return;

	if(world == nullptr)
		return;

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
			
			if(player == nullptr || player->GetController() == nullptr)
				continue;
			
			if(player->GetController()->IsPlayerController() && !player->IsHidden())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), player);

				DrawDebugSphere(world, center, SearchRadius, 16, FColor::Red,false, 2.0f);
				
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}
	DrawDebugSphere(world, center, SearchRadius, 16, FColor::Green,false, 2.0f);
}
