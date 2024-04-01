// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/CSLobbyGameMode.h"
#include "LobbyWidget.h"


void ACSLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// lobby widget ���� viewport �� ������
	lobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), lobbyWidgetFactory);
	lobbyWidget->AddToViewport();

	// ���콺 ������ ����
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}