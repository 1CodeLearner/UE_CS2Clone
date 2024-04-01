// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSLobbyGameMode.generated.h"

/**
 *
 */
class ULobbyWidget;

UCLASS()
class CS2CLONE_API ACSLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyWidget> lobbyWidgetFactory;
	UPROPERTY()
	class ULobbyWidget* lobbyWidget;
};
