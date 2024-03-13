// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Base/CInteractableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "InventoryComponent.h"

ACInteractableItem::ACInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComp;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SMComp->SetupAttachment(RootComponent);
}

void ACInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACInteractableItem::OnOverlapBegin);

	UE_LOG(LogTemp, Warning, TEXT("Tag Name: %s"), *WeaponNameTag.GetTagName().ToString());

	if (ensure(DT_Weapons))
	{
		FItemType* Info = DT_Weapons->FindRow<FItemType>(WeaponNameTag.GetTagName(), FString::Printf(TEXT("")));
		if (ensure(Info))
		{
			ItemInfo = *Info;
		}
	}
}

void ACInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UInventoryComponent* inven = OtherActor->GetComponentByClass<UInventoryComponent>();
}

