// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_CanAttack.h"

#include "AIController.h"
#include "MyEnemyCharacter.h"
#include "MyGameInstance.h"
#include "MyPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTDeco_CanAttack::UBTDeco_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

void UBTDeco_CanAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	auto myGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(UBTNode::GetWorld()));

	if(myGameInstance)
		_attackAbleRange = myGameInstance->GetAIInfoData("Remote")->attackAbleRange;
}

bool UBTDeco_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto currentPawn = Cast<AMyEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(currentPawn == nullptr)
	{
		return false;
	}

	auto target = Cast<AMyPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if(target == nullptr)
	{
		return false;
	}
	
	return bResult && target->GetDistanceTo(currentPawn) <= _attackAbleRange;
}
