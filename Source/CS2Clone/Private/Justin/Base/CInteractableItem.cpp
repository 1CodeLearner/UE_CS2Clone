// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Base/CInteractableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "InventoryComponent.h"
#include "MyCharacter.h"
#include "CSGameInstance.h"

ACInteractableItem::ACInteractableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	//RootComponent = BoxComp;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	//SMComp->SetupAttachment(RootComponent);
	RootComponent = SMComp;
	BoxComp->SetupAttachment(SMComp);
}

void ACInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACInteractableItem::OnOverlapBegin);

	UE_LOG(LogTemp, Warning, TEXT("Tag Name: %s"), *ItemNameTag.GetTagName().ToString());

	if (ensure(DT_Items))
	{
		FItemType* Info = DT_Items->FindRow<FItemType>(ItemNameTag.GetTagName(), FString::Printf(TEXT("")));
		if (ensure(Info))
		{
			ItemInfo = *Info;

		}
	}
	//ItemInfo.ItemNameTag == ItemNameTag;
	//EInventorySlotType::GetDisplayNameTextByIndex
	//ItemInfo.ItemNameTag.GetTagName().
	SMComp->SetSimulatePhysics(true);
}

void ACInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//FItemType* SlotType = DT_Items->FindRow<FItemType>(ItemNameTag.GetTagName(), FString::Printf(TEXT("")));
	//int32 ConversionInfo = (int32)SlotType;
	//int32 enum 캐스트 
	//인벤토리 컴포넌트 가져오기
	UInventoryComponent* inven = OtherActor->GetComponentByClass<UInventoryComponent>();
	// otherActor에 player추가 => 플레이어랑 충돌나면
	AMyCharacter* player = Cast<AMyCharacter>(OtherActor);

	
	UCSGameInstance* gameInstance = Cast<UCSGameInstance>(GetWorld()->GetGameInstance());
	//플레이어랑 충돌 났으면 => 나중에 인벤토리에 아이템이 들어가 있는지 없는지 판단
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Item"));
		//인벤에 이 아이템 정보 넣어라
		bool isGetItem = player->GetItem(ItemInfo);
		if(isGetItem)Destroy();

	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NO Get Item"));
	}
}

