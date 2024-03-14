// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayDisplay.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class CS2CLONE_API UGameplayDisplay : public UUserWidget
{
	GENERATED_BODY()
	
	void SetTime(float time);

public: 
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimeText;

};
