// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWorldWidget.generated.h"

/**
 *
 */
class UTextBlock; 
UCLASS()
class CS2CLONE_API UBaseWorldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Display;
	UPROPERTY()
	FVector3d OffsetLocation;
};
