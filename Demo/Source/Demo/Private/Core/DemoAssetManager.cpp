// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DemoAssetManager.h"
#include "AbilitySystemGlobals.h"

void UDemoAssetManager::StartInitialLoading() {
	Super::StartInitialLoading();
	//mandatory call to use TargetData
	UAbilitySystemGlobals::Get().InitGlobalData();

	UE_LOG(DemoAssetManagerLog, Log, TEXT("using AssetManager"));
}