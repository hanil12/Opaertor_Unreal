// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MyMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API UBTTask_MyMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool SearchTarget(UBehaviorTreeComponent& OwnerComp);

private:
	float _detectRange;
	float _attackAbleRange;
};
