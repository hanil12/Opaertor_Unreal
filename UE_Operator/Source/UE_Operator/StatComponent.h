// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FHpChangedRatio, float)

USTRUCT()
struct FCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxMp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 attack;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_OPERATOR_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	virtual void InitializeComponent() override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetStatByLevel(int32 level);
	void SetHp(int32 hp);
	// TODO : SetAtk, SetMP

	UFUNCTION()
	void Damaged(int32 amount);

	int32 GetLevel() const { return _level; }
	int32 GetCurHp() const { return _curHp; }
	int32 GetMaxHp() const { return _maxHp; }
	int32 GetCurMp() const { return _curMp; }
	int32 GetMaxMp() const { return _maxMp; }
	int32 GetAtk()   const { return _atk; }

	float GetHpRatio();

	
	FHpChangedRatio _onHpChangedRatio;
	
private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _level;
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _curHp;
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _maxHp;
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _curMp;
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _maxMp;
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	int32 _atk;
};
