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
	//ItemInfo.ItemNameTag == WeaponNameTag;
}

void ACInteractableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//FItemType* SlotType = DT_Weapons->FindRow<FItemType>(WeaponNameTag.GetTagName(), FString::Printf(TEXT("")));
	//int32 ConversionInfo = (int32)SlotType;

	//�κ��丮 ������Ʈ ��������
	UInventoryComponent* inven = OtherActor->GetComponentByClass<UInventoryComponent>();
	// otherActor�� player�߰� => �÷��̾�� �浹����
	AMyCharacter* player = Cast<AMyCharacter>(OtherActor);

	
	UCSGameInstance* gameInstance = Cast<UCSGameInstance>(GetWorld()->GetGameInstance());
	//�÷��̾�� �浹 ������ => ���߿� �κ��丮�� �������� �� �ִ��� ������ �Ǵ�
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("choong"));
		// �������
		Destroy();
		
		//�κ��� �� ������ ���� �־��
		//player->GetItem((EInventorySlotType) EInventorySlotType::INV_PRIMARY);
		//player->GetItem(SlotType);
		//inven->myItems.Add(gameInstance->defineItem[]);
		// �������� ���� / ������ ������ ������, ItemNameTag�� (FGameplayTag)
		// ������ �� �ε����� ����
		// break �ض� 

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("xxxxx"));
	}
}

