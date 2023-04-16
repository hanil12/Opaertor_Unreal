// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_MyWait.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API UBTTask_MyWait : public UBTTask_Wait
{
	GENERATED_BODY()
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	bool SearchTarget(UBehaviorTreeComponent& OwnerComp);
};
