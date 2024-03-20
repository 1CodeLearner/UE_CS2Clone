// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Weapon/CHandgun.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Justin/ItemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

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

	//총 초기화
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


	//총 정보 출력
	LogGunState();

	currTime += DeltaSeconds;
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
	}
}

void ACHandgun::LogGunState()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
		FString::Printf(TEXT("Owner: %s"), *GetNameSafe(GetOwner())));

	if (GetWorld()->IsNetMode(NM_Client))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Client - Owner: %s"), *GetNameSafe(GetOwner())));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Server - Owner: %s"), *GetNameSafe(GetOwner())));
	}

	//소지품에 남은 총알 
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Reserve: %d"), ReserveTotalRounds));
	//탄창 총 양
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Total: %d"), InMagTotalRounds));
	//탄창안에 남은 종알
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
		FString::Printf(TEXT("Remaining: %d"), InMagRemainingRounds));
}

bool ACHandgun::CanReload() const
{
	//탄창이 채워지지 않았을 경우 && 예비 탄약이 있을 경우 && 재장전 하고 있지 않은 경우
	return InMagRemainingRounds != InMagTotalRounds && ReserveTotalRounds != 0 && !bClientReloading;
}

//이 함수로 장전
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
	//탄창에 총알이 있을때 && reloading 하고 있지 않을때
	return InMagRemainingRounds > 0 && !bClientReloading;
}

//이 함수로 쏜다
void ACHandgun::Fire()
{
	FHitResult Hit;
	AActor* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CanFire() && ensure(FireAnimSeq) && camera)
	{
		//총 쏘는 애니매이션
		SKMComponent->PlayAnimation(FireAnimSeq, false);

		FVector Start = camera->GetActorLocation();
		FVector End = Start + camera->GetActorForwardVector() * 50000.f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner() ? GetOwner() : this);

		//AMyCharacter를 찾는 Line trace 
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, params);
		AMyCharacter* character = nullptr;
		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("HitActor: %s"), *GetNameSafe(Hit.GetActor())));
			//if hit actor was player, send a server RPC
			character = Cast<AMyCharacter>(Hit.GetActor());
		}

		//발사 되어서 서버에 총 정보 처리하기
		if (HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Server?: %s"), *GetNameSafe(GetOwner()));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Client?: %s"), *GetNameSafe(GetOwner()));
		}
		Server_Fire(character, Hit);
		DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, 2.f);
	}
}

void ACHandgun::Server_Fire_Implementation(AActor* ActorHit, FHitResult Hit)
{
	InMagRemainingRounds--;

	if (InMagRemainingRounds < 0)
		InMagRemainingRounds = 0;
	Multi_Fire();
}

void ACHandgun::Multi_Fire_Implementation()
{
	SKMComponent->PlayAnimation(FireAnimSeq, false);
}
