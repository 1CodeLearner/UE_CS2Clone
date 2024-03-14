// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerController.h"
#include "Justin/UI/GameplayDisplay.h"

AMyPlayerController::AMyPlayerController()
{
	GameplayDisplayClass = nullptr;
	GameplayDisplay = nullptr;
}

void AMyPlayerController::DisplayGameplay()
{
	if (!GameplayDisplay && ensure(GameplayDisplayClass))
	{
		GameplayDisplay = CreateWidget<UGameplayDisplay>(GetWorld(), GameplayDisplayClass);
	}
	if (GameplayDisplay)
	{
		GameplayDisplay->AddToViewport();
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	DisplayGameplay();
}
