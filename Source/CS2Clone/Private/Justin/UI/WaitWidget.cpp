// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/UI/WaitWidget.h"
#include "Components/Button.h"
#include "Justin/Framework/WaitPlayerController.h"

void UWaitWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

	Button_StartGame->OnClicked.AddDynamic(this, &UWaitWidget::OnStartGamePressed);
}

void UWaitWidget::OnStartGamePressed()
{
	auto PC = GetOwningPlayer<AWaitPlayerController>();
	if (PC) {
		PC->StartGame();
	}
}
