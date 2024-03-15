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

	//Spawn �Ҷ� DataTable row name �� ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FGameplayTag ItemNameTag;

	//player UI �� �� �ܾ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText DisplayName;

	//�ι��丮 ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EInventorySlotType InventorySlotType;

	//�ι��丮 �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> InventoryImage;

	//���� �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> StoreImage;

	//�÷��̾ ������ pickup �ϴ� Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACInteractableItem> DroppedItemClass;

	//�÷��̾ ���� ���� �ϴ� Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ACGameplayItem> GameplayItemClass;
};

UENUM(Blueprintable)
enum class EWeaponType
{
	NONE UMETA(DisplayName="None"),
	PISTOL UMETA(DisplayName="Pistol")
};

//���� actor ���� ���� ����
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

	//���� �Ѿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int ReserveTotalRounds;

	//źâ�� ����ִ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int InMagTotalRounds;
};