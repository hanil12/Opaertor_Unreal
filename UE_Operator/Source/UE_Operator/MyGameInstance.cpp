// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "ACharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> dataTable(TEXT("DataTable'/Game/Data/StatDataTable.StatDataTable'"));
	_dataTable = dataTable.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	_emitterTable.Add("Death") = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/ParagonMorigesh/FX/Particles/Morigesh/Abilities/Ultimate/FX/P_Morigesh_Ultimate_Reveal.P_Morigesh_Ultimate_Reveal'"));
	_emitterTable.Add("AttackHit") = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/ParagonMorigesh/FX/Particles/Morigesh/Abilities/Primary/FX/P_Morigesh_Primary_HitCharacter.P_Morigesh_Primary_HitCharacter'"));
}

FCharacterStat* UMyGameInstance::GetStatData(int32 level)
{
	FName name = *FString::FromInt(level);
	FCharacterStat* result = _dataTable->FindRow<FCharacterStat>(name, TEXT(""));
	
	return result;
}

void UMyGameInstance::PlayEffect(FString name, FVector pos)
{
	if(!_emitterTable.Contains(name))
	{
		UE_LOG(LogTemp, Error, TEXT("%s cant find in emitterTable"), *name);
		return;
	}
	
	UGameplayStatics::SpawnEmitterAtLocation(this, *_emitterTable.Find(name) , pos, FRotator::ZeroRotator, true);
}
