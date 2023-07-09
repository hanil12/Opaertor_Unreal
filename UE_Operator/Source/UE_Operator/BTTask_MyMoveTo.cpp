// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MyMoveTo.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "MyGameInstance.h"
#include "MyPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTTask_MyMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	auto myGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(UBTNode::GetWorld()));

	if(myGameInstance)
	{
		_detectRange = myGameInstance->GetAIInfoData("Remote")->detectRange;
		_attackAbleRange = myGameInstance->GetAIInfoData("Remote")->attackAbleRange;
	}
}

void UBTTask_MyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto player = Cast<AMyPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if(!player)
		return;

	float distance = FVector::Distance(player->GetActorLocation(), currentPawn->GetActorLocation());

	if (distance < (_attackAbleRange - 20))
	{
		AbortTask(OwnerComp, NodeMemory);
	}
	else if(distance > _detectRange + 20)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		AbortTask(OwnerComp, NodeMemory);
	}

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

bool UBTTask_MyMoveTo::SearchTarget(UBehaviorTreeComponent& OwnerComp)
{
	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	UWorld* world = currentPawn->GetWorld();

	if(currentPawn == nullptr)
		return false;

	if(world == nullptr)
		return false;

	FVector center = currentPawn->GetActorLocation();

	float SearchRadius =  _detectRange;

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
