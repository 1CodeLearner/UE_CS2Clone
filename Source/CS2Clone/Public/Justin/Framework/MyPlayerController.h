// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 *
 */

class UGameplayDisplay;

UCLASS()
class CS2CLONE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	void DisplayGameplay();
protected:
	virtual void BeginPlay() override; 

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI")
	TSubclassOf<UGameplayDisplay> GameplayDisplayClass;
	UPROPERTY()
	TObjectPtr<UGameplayDisplay> GameplayDisplay;

};
