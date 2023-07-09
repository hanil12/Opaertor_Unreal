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
public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
	bool SearchTarget(UBehaviorTreeComponent& OwnerComp);

private:
	float _detectRange;
};
