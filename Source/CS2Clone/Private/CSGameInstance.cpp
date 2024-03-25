// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameInstance.h"
#include "InventoryComponent.h"
UCSGameInstance::UCSGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Justin/Data/DT_ItemType.DT_ItemType'"));
	if (tempDT.Succeeded())
	{
		UDataTable* dataTable = tempDT.Object;
		
		//전체 열 가져오기
		TArray<FName> rowNames = dataTable->GetRowNames();
		//게임 인스턴스에 아이템정의 배열 5칸 셋팅
		defineItem.Init(FItemType(), 5);

		//열 이름의 수만큼 반복
		for (int i = 0; i < rowNames.Num(); i++)
		{
			// itemData는 테이블에서 열 찾는 값 / 테이블에서 열 찾아서 itemData에 담음
			FItemType* itemData = dataTable->FindRow<FItemType>(rowNames[i], TEXT(""));

			//defineItem.Add(*itemData);
			//아이템정의 배열에 itemData를 순차적으로 집어넣기 
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
