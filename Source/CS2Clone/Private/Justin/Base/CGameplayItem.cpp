// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Base/CGameplayItem.h"
#include "Components/SkeletalMeshComponent.h"

ACGameplayItem::ACGameplayItem()
{
	PrimaryActorTick.bCanEverTick = true;
	SKMComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SKMComponent");
	RootComponent = SKMComponent;
}

void ACGameplayItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACGameplayItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

