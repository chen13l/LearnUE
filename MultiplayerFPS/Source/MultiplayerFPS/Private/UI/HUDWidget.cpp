// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"
#include "Components\ProgressBar.h"

void UHUDWidget::UpdateHealthPercent(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
}

void UHUDWidget::UpdateArmorPercent(float ArmorPercent)
{
	ArmorBar->SetPercent(ArmorPercent);
}
