// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameInstance.h"
#include "InventoryComponent.h"
UCSGameInstance::UCSGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Justin/Data/DT_ItemType.DT_ItemType'"));
	if (tempDT.Succeeded())
	{
		UDataTable* dataTable = tempDT.Object;
		
		//��ü �� ��������
		TArray<FName> rowNames = dataTable->GetRowNames();
		//���� �ν��Ͻ��� ���������� �迭 5ĭ ����
		defineItem.Init(FItemType(), 5);

		//�� �̸��� ����ŭ �ݺ�
		for (int i = 0; i < rowNames.Num(); i++)
		{
			// itemData�� ���̺��� �� ã�� �� / ���̺��� �� ã�Ƽ� itemData�� ����
			FItemType* itemData = dataTable->FindRow<FItemType>(rowNames[i], TEXT(""));

			//defineItem.Add(*itemData);
			//���������� �迭�� itemData�� ���������� ����ֱ� 
			defineItem[i] = *itemData;
		}
	}
}

void UCSGameInstance::Init()
{
	Super::Init();
}

//void UCSGameInstance::SetGameStarted(bool bStarted)
//{
//	bGameStarted = bStarted;
//}
//
//bool UCSGameInstance::GetGameStarted() const
//{
//	return bGameStarted;
//}
