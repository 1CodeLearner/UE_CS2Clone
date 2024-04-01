// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/WaitPlayerController.h"
#include "Justin/UI/WaitWidget.h"

void AWaitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IsLocalController()) {
		WaitWidget = CreateWidget<UWaitWidget>(this, WaitWidgetClass);
		WaitWidget->AddToViewport();
	}
}

void AWaitPlayerController::StartGame()
{
	Server_StartGame();
}

void AWaitPlayerController::Server_StartGame_Implementation()
{
	UE_LOG(LogTemp,Warning, TEXT("Pressed in server?"));
	GetWorld()->ServerTravel(TEXT("/Game/Level_Gameplay?listen"),true);
}
