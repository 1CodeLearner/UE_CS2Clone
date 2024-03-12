// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.generated.h"

class ACGameplayActor;
class ACInteractableItem;
class UTexture2D;


UENUM(BlueprintType)
enum class EInventorySlotType
{
	INV_PRIMARY UMETA(DisplayName="Primary"),
	INV_SECONDARY UMETA(DisplayName="Secondary"),
	INV_MELEE UMETA(DisplayName="Melee"),
	INV_UTILITY UMETA(DisplayName="Utility"),
	INV_BOMB UMETA(DisplayName="Bomb"),
	INV_MAX UMETA(DisplayName="Max")
};

USTRUCT(BlueprintAble)
struct FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FItemInfo()
	{
		DisplayName = FText::FromString(FString::Printf(TEXT("")));
		InventorySlotType = EInventorySlotType::INV_MAX;
		InventoryImage = nullptr;
		StoreImage = nullptr;
		DroppedItemClass = nullptr;
		ItemBaseClass = nullptr;
	}

	//Spawn 할때 DataTable row name 에 쓰여진다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGameplayTag WeaponTag;

	//UI에 쓸 단어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText DisplayName;

	//인밴토리 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EInventorySlotType InventorySlotType;

	//인밴토리 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> InventoryImage;

	//상점 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> StoreImage;

	//플레이어가 땅에서 pickup 하는 Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACInteractableItem> DroppedItemClass;

	//플레이어가 집적 handle 하는 Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACGameplayActor> ItemBaseClass;

};

UENUM(BlueprintType)
enum class EWeaponType
{
	HANDGUN UMETA(DisplayName="Primary"),
	MAX UMETA(DisplayName="MAX")
};

USTRUCT(BlueprintAble)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FWeaponInfo() 
	{
		WeaponType = EWeaponType::MAX;
		ReserveTotalRounds = 0; 
		InMagTotalRounds = 0;
	}

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere)
	int ReserveTotalRounds;
	UPROPERTY(EditAnywhere)
	int InMagTotalRounds;

};