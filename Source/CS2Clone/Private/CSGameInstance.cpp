// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameInstance.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Online/OnlineSessionNames.h>


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

	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		// ���� �������̽� ��������
		UE_LOG(LogTemp, Warning, TEXT("Steam session success"));
		 
		// ���� �������̽� ��������
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCSGameInstance::OnCreateSessionComplete);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCSGameInstance::OnDestroySessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCSGameInstance::OnFindSessionComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCSGameInstance::OnJoinSessionComplete);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Steam session unsuccessful"));
	}
}


void UCSGameInstance::CreateMySession(FString roomName, int32 maxPlayer)
{
	FOnlineSessionSettings sessionSettings;

	// true ������ �˻� �ȴ�.
	sessionSettings.bShouldAdvertise = true;

	// steam ����ϸ� �ش� �ɼ��� true ������ ���� �� �ִ�.
	sessionSettings.bUseLobbiesIfAvailable = true;

	// ���� �������� �ƴ����� �����ٰ���
	sessionSettings.bUsesPresence = true;
	// ���� �÷��� �߿� ������ �� �ְ�
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// �ο� �� 
	sessionSettings.NumPublicConnections = maxPlayer;

	// base64�� Encode
	roomName = StringBase64Encode(roomName);
	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	// ���� ���� ��û
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	int32 rand = FMath::RandRange(1, 100000);
	mySessionName += FString::Printf(TEXT("%d"), rand);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);

	UE_LOG(LogTemp, Warning, TEXT("SessionCreated: %s"), *mySessionName);
}

void UCSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());
		// Battle Map ���� �̵�����
		GetWorld()->ServerTravel(TEXT("/Game/Level_Wait?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}

void UCSGameInstance::DestroyMySession()
{
	sessionInterface->DestroySession(FName(mySessionName));
}

void UCSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

void UCSGameInstance::FindOtherSession()
{
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionSearch->MaxSearchResults = 10;

	// ���� �˻� ��û
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UCSGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Success - count : %d"), results.Num());

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult si = results[i];
			FString roomName;
			si.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);

			// ���� ���� ---> String ���� 
			// ������ �ִ� �ο�
			int32 maxPlayer = si.Session.SessionSettings.NumPublicConnections;
			// ������ ���� �ο� (�ִ� �ο� - ���� �ο�)

			int32 currPlayer = maxPlayer - si.Session.NumOpenPublicConnections;

			roomName = StringBase64Decode(roomName);
			// ���̸� ( 5 / 10 )
			FString sessionInfo = FString::Printf(
				TEXT("%s ( %d / %d )"),
				*roomName, currPlayer, maxPlayer);

			onSearchComplete.ExecuteIfBound(i, sessionInfo);
		}

		// idx �� -1 �����ؼ� �˻� �Ϸ� �˷�����
		onSearchComplete.ExecuteIfBound(-1, TEXT(""));


		/*for (auto si : results)
		{
			FString roomName;
			si.Session.SessionSettings.Get(FName(TEXT("ROOM_NAME")), roomName);
		}*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Fail"));
	}
}

void UCSGameInstance::JoinOtherSession(int32 idx)
{
	//TArray<FOnlineSessionSearchResult> 
	auto results = sessionSearch->SearchResults;
	if (sessionInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("sessionInterface is null"));
	}
	if (results.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("results Zero"));

	}
	UE_LOG(LogTemp, Warning, TEXT("results count : %d, idx : %d"), results.Num(), idx);
	sessionInterface->JoinSession(0, FName(mySessionName), results[idx]);
}

void UCSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success : %s"), *SessionName.ToString());
		FString url;
		// �����ؾ� �ϴ� Listen ���� URL�� �޾� ����
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);

		if (!url.IsEmpty())
		{
			// �ش� URL �� ��������
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Fail : %d"), result);
	}
}

FString UCSGameInstance::StringBase64Encode(FString str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UCSGameInstance::StringBase64Decode(FString str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}


void UCSGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCSGameInstance, bGameOngoing);
}

void UCSGameInstance::SetGameOnGoing(bool bStarted)
{
	bGameOngoing = bStarted;
}

bool UCSGameInstance::IsGameOnGoing() const
{
	return bGameOngoing;
}
