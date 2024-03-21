#include "MyCharacter.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/SkeletalMesh.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "InventoryComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Justin/Base/CInteractableItem.h"
#include "CSGameInstance.h"
#include "Justin/Weapon/CHandgun.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//skeleta mesh 셋팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm->SetupAttachment(RootComponent);
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);
	camera->SetWorldLocation(FVector(30, 0, 80));

	//invencomp
	invenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));

	GunComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Gun"));
	GunComponent->SetupAttachment(GetMesh(), FName(TEXT("GunPosition")));
	GunComponent->SetRelativeLocation(FVector(-7.144f, 3.68f, 4.136f));
	GunComponent->SetRelativeRotation(FRotator(3.4f, 75.699f, 6.6424f));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//get APlayerController
	APlayerController* playerContoller = Cast<APlayerController>(GetController());
	//get subSystem
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());
	subSystem->AddMappingContext(imc_Default, 0);

	hasPistol = false;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_Move, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedMove);
		enhancedInputComponent->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedJump);
		enhancedInputComponent->BindAction(ia_Look, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedLook);
		enhancedInputComponent->BindAction(ia_GetDrop, ETriggerEvent::Started, this, &AMyCharacter::EnhancedDrop);
		enhancedInputComponent->BindAction(ia_InputItemSlot, ETriggerEvent::Started, this, &AMyCharacter::SelectItem);
		enhancedInputComponent->BindAction(ia_Fire, ETriggerEvent::Started, this, &AMyCharacter::EnhancedFire);
		enhancedInputComponent->BindAction(ia_Reload, ETriggerEvent::Started, this, &AMyCharacter::EnhancedReload);
	}
}

void AMyCharacter::EnhancedMove(const struct FInputActionValue& value)
{	
	FVector2D dir = value.Get<FVector2D>();
	FVector originVec = FVector(dir.Y, dir.X, 0);
	FVector newVec = GetTransform().TransformVector(originVec);

	AddMovementInput(newVec);
}

void AMyCharacter::EnhancedJump(const struct FInputActionValue& value)
{
	Jump();
}

void AMyCharacter::EnhancedLook(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();

	AddControllerYawInput(dir.X);
	AddControllerPitchInput(dir.Y);

}

void AMyCharacter::EnhancedDrop(const struct FInputActionValue& value)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Drop Item"));
	
	DetachGun();
}

void AMyCharacter::SelectItem(const struct FInputActionValue& value)
{
	//input 입력값 가져와
	int32 slotIdx = value.Get<float>();
	slotIdx--;

	//슬롯 인덱스와 내 아이템 연동 / 인벤토리슬롯으로
	EInventorySlotType HaveItemSlot = invenComp->myItems[slotIdx].InventorySlotType;

	// 내 아이템 슬롯에 아이템이 없으면 리턴
	if(HaveItemSlot == EInventorySlotType::INV_MAX) return;
	
	UE_LOG(LogTemp, Warning, TEXT("item : %d"), HaveItemSlot);
	
	//선택한 아이템의 기능을 사용할 수 있는 함수를 호출
	AttachGun();
}

void AMyCharacter::EnhancedFire(const struct FInputActionValue& value)
{
	//if (hasPistol && invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	PlayerFIre();
}

void AMyCharacter::EnhancedReload(const struct FInputActionValue& value)
{
	PlayerReload();
}

// old version
void AMyCharacter::GetItem(EInventorySlotType slotType)
{
	//FName slotName = UEnum::GetValueAsName<EInventorySlotType>(slotType);
	//UE_LOG(LogTemp, Warning, TEXT("slotName : %s"), *slotName.ToString());
	// 열거형을 인트형으로 형변환
	int32 invenSlotType = (int32)slotType;

	
	// 1. 아이템 슬롯 중복 되면 안되고 인벤 배열이 5이상 되면 들어오지않도록
	
	// 게임 인스턴스 가져오기
	UCSGameInstance* gameInstance = Cast<UCSGameInstance>(GetWorld()->GetGameInstance());
	
	//invenComp->myItems.Add(gameInstance->defineItem[invenSlotType]);
	invenComp->myItems[invenSlotType] = gameInstance->defineItem[invenSlotType];
	// 가지고있는것중에 0보다 크면 return //or nullptr; 

	//게임 인스턴스 가져오기
	//UCSGameInstance* gameInstance = Cast<UCSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//인벤 컴포넌트에
	//invenComp->myItems.Add(gameInstance->defineItem[slotType]);


}

//new version
bool AMyCharacter::GetItem(FItemType itemInfo)
{
	// 인벤토리에서 몇번째 자리에 들어가야하는지?
	// 슬롯타입 정보 담아놓고 형변환
	int32 invenSlotType = (int32)(itemInfo.InventorySlotType);

	//만약 인벤에 있는 아이템 배열에서 아이템 타입이 max가 아니면 false 리턴 
	//같은 슬롯타입에 아이템을 갖고있으면 안됨 
	if(invenComp->myItems[invenSlotType].InventorySlotType != EInventorySlotType::INV_MAX) return false;

	//내 인벤 슬롯 타입에 충돌체 아이템정보 추가
	invenComp->myItems[invenSlotType] = itemInfo; 
	return true;
}


void AMyCharacter::AttachGun()
{
	// 2번 권총 슬롯이 비어있으면 리턴
	if(invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	
	//핸드건 가지고 있지 않으면
	if (!hasPistol) // true면 내려가 
	{
		// 핸드건 스폰
		handGun = GetWorld()->SpawnActor<ACHandgun>(invenComp->myItems[1].GameplayItemClass, GetActorLocation(), GetActorRotation());
		if(handGun != nullptr)
		// 핸드건을 건 컴포넌트에 붙이기
		handGun->AttachToComponent(GunComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

		//핸드건 가지고있다
		hasPistol = true;
	}

}

// 총 떨구기와 떨군 총 스폰
void AMyCharacter::DetachGun()
{
	if (invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;

	// 총 가지고 있으면
	if (hasPistol)
	{
		//총 손에서 제거
		handGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		//필드에 총 스폰
		GetWorld()->SpawnActor<ACInteractableItem>(invenComp->myItems[1].DroppedItemClass, GetActorLocation(), GetActorRotation());
		//비어있는 슬롯으로 바꿔치기
		invenComp->myItems[1] = invenComp->myItems[0];

		//총 안가지고있다
		hasPistol = false;
	}
}

void AMyCharacter::PlayerFIre()
{
	//if(handGun == nullptr) return;
	//hasPistol이 true 면 내려가고 false 리턴
	if(!hasPistol) return; 
	
	handGun->Fire();
}

void AMyCharacter::PlayerReload()
{
	//if (hasPistol && invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	if(!hasPistol) return; 
	handGun->Reload();
}

