// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Justin/ItemTypes.h"
#include "GameplayTagContainer.h"
#include "CInteractableItem.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UDataTable; 

UCLASS(Abstract)
class CS2CLONE_API ACInteractableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACInteractableItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UBoxComponent> BoxComp;

	//FGameplayTag �� FName ���� DataTable(FItemType) ������ ���´� 
	UPROPERTY(EditAnywhere, Category="Settings | Items")
	FGameplayTag ItemNameTag;

	//DataTable of FItemType 
	UPROPERTY(EditAnywhere, Category= "Settings | Items")
	TObjectPtr<UDataTable> DT_Items;

	//������ �����ϰ� �÷��̾�� �����Ҷ� �����̾� �κ��丮�� ����
	UPROPERTY(VisibleAnywhere, Category= "Settings | Items")
	FItemType ItemInfo;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	UPROPERTY(EditAnywhere)
	float TakeItemDistance = 200;

	UPROPERTY(EditAnywhere)
	EInventorySlotType SlotType;
};
