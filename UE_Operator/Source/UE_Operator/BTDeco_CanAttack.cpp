// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_CanAttack.h"

#include "AIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDeco_CanAttack::UBTDeco_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDeco_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto currentPawn = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(currentPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Cant attack1"));
		return false;
	}

	auto target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if(target == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Cant attack2"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Can attack"));
	return bResult && target->GetDistanceTo(currentPawn) <= 1000.0f;
}
