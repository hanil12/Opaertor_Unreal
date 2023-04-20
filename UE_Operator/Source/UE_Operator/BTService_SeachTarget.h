// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SeachTarget.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API UBTService_SeachTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SeachTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void SearchTarget(UBehaviorTreeComponent& OwnerComp);

private:
	float _detectRange;
};
