// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.generated.h"

class ACWeaponBase;
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
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FWeaponInfo() 
	{
		DisplayName = FText::FromString(FString::Printf(TEXT("")));
		InventorySlotType = EInventorySlotType::INV_MAX;
		InventoryImage = nullptr;
		StoreImage = nullptr;
		DroppedWeaponClass = nullptr;
		WeaponBaseClass = nullptr;
		ReserveTotalRounds = 0;
		InMagTotalRounds = 0;
	}

	//Spawn할때 DataTable row name 에 쓰여진다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FGameplayTag WeaponTag;

	//UI에 쓸 단어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FText DisplayName;

	//인밴토리 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EInventorySlotType InventorySlotType;

	//인밴토리 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UTexture2D> InventoryImage;

	//상점 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UTexture2D> StoreImage;

	//플레이어가 땅에서 pickup 하는 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ACInteractableItem> DroppedWeaponClass;

	//플레이어가 집적 handle 하는 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ACWeaponBase> WeaponBaseClass;

	//남은 총알
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int ReserveTotalRounds;

	//탄창에 들어있는 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int InMagTotalRounds;
};

