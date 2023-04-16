// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpWidget.h"

#include "StatComponent.h"
#include "Components/ProgressBar.h"

void UMyHpWidget::BindHp(UStatComponent* statComponent)
{
	statComponent->_onHpChangedRatio.AddUObject(this, &UMyHpWidget::SetHpBarRatio);
	SetHpBarRatio(statComponent->GetHpRatio());
}

void UMyHpWidget::SetHpBarRatio(float value)
{
	PB_HpBar->SetPercent(value);
}
