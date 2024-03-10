// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Base/CWeaponBase.h"
#include "EngineUtils.h"
#include "Justin/Base/CInteractableItem.h"
#include "Net/UnrealNetwork.h"

ACWeaponBase::ACWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ACWeaponBase::CanReload() const
{
	return InMagRemainingRounds != InMagTotalRounds && ReserveTotalRounds != 0;
}

void ACWeaponBase::Reload()
{
	ServerReload();
}

void ACWeaponBase::ServerReload_Implementation()
{
	ensure(CanReload());

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
}

bool ACWeaponBase::CanFire() const
{
	return InMagRemainingRounds > 0;
}

void ACWeaponBase::Fire()
{
	ensure(CanFire());

	InMagRemainingRounds--;

	if (InMagRemainingRounds < 0)
		InMagRemainingRounds = 0;
}

void ACWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME_CONDITION(ACWeaponBase, ReserveTotalRounds, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACWeaponBase, InMagTotalRounds, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACWeaponBase, InMagRemainingRounds, COND_OwnerOnly);*/
}

void ACWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Owner: %s"), *GetNameSafe(GetOwner())));

	if (GetWorld()->IsNetMode(NM_Client))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Client")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Server")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Reserve: %d"), ReserveTotalRounds));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Total: %d"), InMagTotalRounds));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Remaining: %d"), InMagRemainingRounds));

}
