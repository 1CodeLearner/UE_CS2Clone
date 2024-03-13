// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameInstance.h"

UCSGameInstance::UCSGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Justin/Data/DT_WeaponInfo.DT_WeaponInfo'"));
	if (tempDT.Succeeded())
	{
		UDataTable* dataTable = tempDT.Object;

		//��ü �� ��������
		TArray<FName> rowNames = dataTable->GetRowNames();
		// �� ���������� �߰�
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
