// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();

	virtual void Init() override;

	FCharacterStat* GetStatData(int32 level);
	FAIInfo* GetAIInfoData(FString type);

	void PlayEffect(FString name, FVector pos);
	
private:
	UPROPERTY()
	class UDataTable* _statDataTable;

	UPROPERTY()
	class UDataTable* _aiDataTable;

	UPROPERTY()

	TMap<FString, UParticleSystem*> _emitterTable;
};
