// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Base/CGameplayActor.h"
#include "Components/SkeletalMeshComponent.h"

ACGameplayActor::ACGameplayActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SKMComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SKMComponent");
	RootComponent = SKMComponent;
}

void ACGameplayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACGameplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

