// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "CSGameInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 플레이어 인벤 비어있는 5칸 생성 
	myItems.Init(FItemType(), 5);

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();





	//for (int32 i = 0; i < myItems.Num(); i++)
	//{
	//	myItems.Add(FItemType.);
	//}
	
	
	//myItems.Add((FItemType::InventorySlotType));
	
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

