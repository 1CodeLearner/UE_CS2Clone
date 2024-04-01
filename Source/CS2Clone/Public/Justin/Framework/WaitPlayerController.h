// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaitPlayerController.generated.h"

/**
 *
 */
 class UWaitWidget;

UCLASS()
class CS2CLONE_API AWaitPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void StartGame();

	UFUNCTION(Server, Reliable)
	void Server_StartGame();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UWaitWidget> WaitWidgetClass;
	UPROPERTY()
	TObjectPtr<UWaitWidget> WaitWidget;
};
