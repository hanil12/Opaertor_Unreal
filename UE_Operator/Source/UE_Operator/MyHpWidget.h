// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API UMyHpWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindHp(class UStatComponent* statComponent);
	void SetHpBarRatio(float value);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;
};
