// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/UI/GameplayDisplay.h"
#include "Components/TextBlock.h"

void UGameplayDisplay::SetTime(float time)
{
	TimeText->SetText(FText::AsNumber(time));
}
