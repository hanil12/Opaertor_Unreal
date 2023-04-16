// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "AIController.h"
#include "MyEnemyCharacter.h"
#include "MyPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Attack! IN"));
	auto myCharacter = Cast<AMyEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	auto player = Cast<AMyPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	
	if(myCharacter == nullptr || player == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector playerLocation = player->GetActorLocation();
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(myCharacter->GetActorLocation(), playerLocation);
	
	myCharacter->SetActorRotation(rotator);
	myCharacter->Attack();
	UE_LOG(LogTemp, Log, TEXT("Attack!!!"));
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
