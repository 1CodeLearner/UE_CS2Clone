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
		enhancedInputComponent->BindAction(ia_GetDrop, ETriggerEvent::Started, this, &AMyCharacter::EnhancedGetDrop);
		enhancedInputComponent->BindAction(ia_InputItemSlot, ETriggerEvent::Started, this, &AMyCharacter::InputItemSlot);
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

void AMyCharacter::EnhancedGetDrop(const struct FInputActionValue& value)
{
	
	UE_LOG(LogTemp, Warning, TEXT("f"));
	//getitem()을 실행시킨다
	//GetItem();

}

void AMyCharacter::InputItemSlot(const struct FInputActionValue& value)
{
	//input 입력값 가져와
	int32 slotIdx = value.Get<float>();
	slotIdx--;

	UE_LOG(LogTemp, Warning, TEXT("get value : %d"), slotIdx);
	GetItem((EInventorySlotType)slotIdx);
}

// old version
void AMyCharacter::GetItem(EInventorySlotType slotType)
{
	FName slotName = UEnum::GetValueAsName<EInventorySlotType>(slotType);
	UE_LOG(LogTemp, Warning, TEXT("slotName : %s"), *slotName.ToString());
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
	int32 invenSlotType = (int32)(itemInfo.InventorySlotType);

	if(invenComp->myItems[invenSlotType].InventorySlotType != EInventorySlotType::INV_MAX) return false;

	invenComp->myItems[invenSlotType] = itemInfo;

	return true;
}

