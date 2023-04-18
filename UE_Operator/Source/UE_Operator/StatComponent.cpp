// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetStatByLevel(1);
	SetHp(_maxHp);
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatComponent::SetStatByLevel(int32 level)
{
	auto myGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if(IsValid(myGameInstance))
	{
		auto stat = *myGameInstance->GetStatData(level);
		_level = stat.level;
		_maxHp = stat.maxHp;
		_maxMp = stat.maxMp;
		_atk = stat.attack;
		_curHp = stat.maxHp;
		_maxMp = stat.maxMp;
	}

	SetHp(_curHp);
}

void UStatComponent::SetHp(int32 hp)
{
	if(hp < 0)
	{
		_curHp = 0;
		_death.Broadcast();
	}
	else
		_curHp = hp;

	float hpRatio = GetHpRatio();
	_onHpChangedRatio.Broadcast(hpRatio);
}

void UStatComponent::Damaged(int32 amount)
{
	int32 newHp = _curHp - amount;
	SetHp(newHp);
}

float UStatComponent::GetHpRatio()
{
	if(_maxHp == 0)
		return 0.0f;
	return (float)_curHp / (float)_maxHp;
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

