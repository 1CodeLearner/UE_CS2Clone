// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/UI/GameplayDisplay.h"
#include "Components/TextBlock.h"

void UGameplayDisplay::SetTime(float time)
{
	int32 Minutes = FMath::FloorToInt(time / 60.f);
	float Seconds = time - Minutes * 60.f;
	FString PrintTime = FString::Printf(TEXT("%02d : %02d"), Minutes, FMath::CeilToInt(Seconds));

	TimeText->SetText(FText::FromString(PrintTime));
}

void UGameplayDisplay::SetWinner(FText winnerText)
{
	TimeText->SetText(winnerText);
}
