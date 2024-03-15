// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.generated.h"

class ACGameplayItem;
class ACInteractableItem;
class UTexture2D;


UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{	
	INV_PRIMARY UMETA(DisplayName="Primary") ,
	INV_SECONDARY UMETA(DisplayName="Secondary"),
	INV_MELEE UMETA(DisplayName="Melee"),
	INV_UTILITY UMETA(DisplayName="Utility"),
	INV_BOMB UMETA(DisplayName="Bomb"),
	INV_MAX UMETA(Hidden)
};

USTRUCT(BlueprintAble)
struct FItemType: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FItemType() 
	{
		DisplayName = FText::FromString(FString::Printf(TEXT("")));
		InventorySlotType = EInventorySlotType::INV_MAX;
		InventoryImage = nullptr;
		StoreImage = nullptr;
		DroppedItemClass = nullptr;
		GameplayItemClass = nullptr;
	}

	//Spawn 할때 DataTable row name 에 쓰여진다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGameplayTag ItemNameTag;

	//player UI 에 쓸 단어
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

	//플레이어가 땅에서 pickup 하는 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACInteractableItem> DroppedItemClass;

	//플레이어가 직접 쓰는 하는 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACGameplayItem> GameplayItemClass;
};

UENUM(Blueprintable)
enum class EWeaponType
{
	NONE UMETA(DisplayName="None"),
	PISTOL UMETA(DisplayName="Pistol")
};

//무기 actor 마다 들어가는 정보
USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FWeapon()
	{
		EWeaponType = EWeaponType::NONE;
		ReserveTotalRounds = 0;
		InMagTotalRounds = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType EWeaponType;

	//남은 총알
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int ReserveTotalRounds;

	//탄창에 들어있는 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int InMagTotalRounds;
};