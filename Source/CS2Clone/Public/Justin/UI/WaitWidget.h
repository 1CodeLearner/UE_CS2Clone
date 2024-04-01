// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitWidget.generated.h"

/**
 *
 */

class UButton;

UCLASS()
class CS2CLONE_API UWaitWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_StartGame;
	UFUNCTION()
	void OnStartGamePressed();
};
