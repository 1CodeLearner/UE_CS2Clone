// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Weapon/CHandgun.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Justin/ItemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerState.h"
#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerController.h"
#include "Justin/Framework/MyGameState.h"

ACHandgun::ACHandgun()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ReserveTotalRounds = 0;
	InMagTotalRounds = 0;
	InMagRemainingRounds = 0;
	bClientReloading = false;
}


void ACHandgun::BeginPlay()
{
	Super::BeginPlay();

	//�� �ʱ�ȭ
	if (ensure(DT_Weapon) && ensure(WeaponTag.IsValid()))
	{
		FWeapon* weaponInfo = DT_Weapon->FindRow<FWeapon>(WeaponTag.GetTagName(), FString::Printf(TEXT("")));
		if (weaponInfo)
		{
			ReserveTotalRounds = weaponInfo->ReserveTotalRounds;
			InMagTotalRounds = weaponInfo->InMagTotalRounds;
			InMagRemainingRounds = InMagTotalRounds;
		}
	}

	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//if (ensure(PC)) 
	//{
	//}
}

void ACHandgun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACHandgun, ReserveTotalRounds, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACHandgun, InMagRemainingRounds, COND_OwnerOnly);
}

void ACHandgun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//�� ���� ���
	LogGunState();

	//�½��ÿ� 
	/*currTime += DeltaSeconds;
	if (currTime > timerTotal)
	{
		if (!GetOwner())
		{
			for (TActorIterator<ACharacter> it(GetWorld()); it; ++it)
			{
				SetOwner(*it);
			}
		}
		currTime = timerTotal;
	}*/
}

void ACHandgun::LogGunState()
{
	//�½��ÿ� 
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
	//	FString::Printf(TEXT("Owner: %s"), *GetNameSafe(GetOwner())));

	//if (GetWorld()->IsNetMode(NM_Client))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Client - Owner: %s"), *GetNameSafe(GetOwner())));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Server - Owner: %s"), *GetNameSafe(GetOwner())));
	//}

	////����ǰ�� ���� �Ѿ� 
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Reserve: %d"), ReserveTotalRounds));
	////źâ �� ��
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Total: %d"), InMagTotalRounds));
	////źâ�ȿ� ���� ����
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
	//	FString::Printf(TEXT("Remaining: %d"), InMagRemainingRounds));
}

bool ACHandgun::CanReload() const
{
	//źâ�� ä������ �ʾ��� ��� && ���� ź���� ���� ��� && ������ �ϰ� ���� ���� ���
	return InMagRemainingRounds != InMagTotalRounds && ReserveTotalRounds != 0 && !bClientReloading;
}

//�� �Լ��� ����
void ACHandgun::Reload()
{
	if (CanReload())
	{
		SKMComponent->PlayAnimation(ReloadAnimSeq, false);
		bClientReloading = true;
		Server_Reload();
	}
}

void ACHandgun::Server_Reload_Implementation()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &ACHandgun::CompleteReload, 1.5f, false);
	Multi_Reload();
}

void ACHandgun::Multi_Reload_Implementation()
{
	if (!SKMComponent->IsPlaying())
	{
		SKMComponent->PlayAnimation(ReloadAnimSeq, false);
	}
}

void ACHandgun::CompleteReload()
{
	int refillAmt = InMagTotalRounds - InMagRemainingRounds;

	int ReserveTemp = ReserveTotalRounds - refillAmt;
	if (ReserveTemp < 0)
	{
		refillAmt += ReserveTemp;
		ReserveTotalRounds = 0;
	}
	else
		ReserveTotalRounds = ReserveTemp;

	InMagRemainingRounds += refillAmt;

	//Reset bool for server
	bClientReloading = false;
	Client_CompleteReload();
}

void ACHandgun::Client_CompleteReload_Implementation()
{
	bClientReloading = false;
}

bool ACHandgun::CanFire() const
{
	//źâ�� �Ѿ��� ������ && reloading �ϰ� ���� ������
	return InMagRemainingRounds > 0 && !bClientReloading;
}

//�� �Լ��� ���
void ACHandgun::Fire()
{
	FHitResult Hit;
	//FVector Start;
	//FVector End;
	//auto owner = IsValid(GetOwner());
	//if (owner)
	//{
	//	auto charac = Cast<AMyCharacter>(GetOwner());
	//	if (charac)
	//	{
	//		auto PC = charac->GetController<AMyPlayerController>();
	//		if (ensure(PC))
	//		{
	//			FVector CamLoc;
	//			FRotator CamRot;
	//			PC->GetPlayerViewPoint(CamLoc, CamRot);

	//			Start = CamLoc;
	//			End = CamLoc + 10000.f * CamRot.Vector().Normalize();


	//		}
	//	}

	//}
	
	AActor* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CanFire() && ensure(FireAnimSeq) && camera)
	{
		//�� ��� �ִϸ��̼�
		SKMComponent->PlayAnimation(FireAnimSeq, false);

		FVector Start = camera->GetActorLocation();
		FVector End = Start + camera->GetActorForwardVector() * 50000.f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner() ? GetOwner() : this);
		//AMyCharacter�� ã�� Line trace 
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, params);
		AMyCharacter* character = nullptr;
		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("Actor hit: %s"), *GetNameSafe(Hit.GetActor())));
		}

		//�߻� �Ǿ ������ �� ���� ó���ϱ�
		Server_Fire(Hit);
		DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, 5.f);
	}

}

void ACHandgun::OnRep_Owner()
{
	UE_LOG(LogTemp, Warning, TEXT("Owner Replicated"));
}

void ACHandgun::Server_Fire_Implementation(FHitResult Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HandGun, Net: %s"), GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server"));
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server?"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("client?"));

	//�÷��̾ �¾Ҵٸ�:
	if (Hit.GetActor())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetNameSafe(Hit.GetActor()->GetOwner()));
		auto character = Cast<AMyCharacter>(Hit.GetActor());
		if (character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player hit: %s"), *character->GetPlayerState()->GetUniqueId().ToString());

			//�÷��̾� ü�� �ٿ���.

			//Move this to MyCharacter later
			character->CurrHp -= 20.f;
			if (character->CurrHp <= 0.f)
			{
				auto GM = GetWorld()->GetAuthGameMode<ARealGameMode>();
				if (GM)
				{
					GM->OnPlayerDead(character);
				}
				character->PlayerDead();
			}


		}
	}
	InMagRemainingRounds--;

	if (InMagRemainingRounds < 0)
		InMagRemainingRounds = 0;
	Multi_Fire();
}

void ACHandgun::Multi_Fire_Implementation()
{
	SKMComponent->PlayAnimation(FireAnimSeq, false);
}
