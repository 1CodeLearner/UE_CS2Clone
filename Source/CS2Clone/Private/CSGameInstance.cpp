// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameInstance.h"

UCSGameInstance::UCSGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Justin/Data/DT_WeaponInfo.DT_WeaponInfo'"));
	if (tempDT.Succeeded())
	{
		UDataTable* dataTable = tempDT.Object;

		//전체 열 가져오기
		TArray<FName> rowNames = dataTable->GetRowNames();
		// 열 순차적으로 추가
		for (int i = 0; i < rowNames.Num(); i++)
		{
			FWeaponInfo* itemData = dataTable->FindRow<FWeaponInfo>(rowNames[i], TEXT(""));

			defineItem.Add(*itemData);
		}
	}
}

void UCSGameInstance::Init()
{
	Super::Init();


}
