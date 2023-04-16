// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "AIController.h"
#include "MyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto myCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if(myCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	myCharacter->Attack();
	bIsAttacking = true;
	myCharacter->_attackEnd.AddLambda([this]() { this->bIsAttacking = false; });
	
	return result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
