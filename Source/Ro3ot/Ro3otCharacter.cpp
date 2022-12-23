// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ro3otCharacter.h"

#include "AttackSystem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ARo3otsCharacter

ARo3otCharacter::ARo3otCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void ARo3otCharacter::BeginPlay()
{
	Super::BeginPlay();

	Hp = MaxHp;
}


void ARo3otCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	attackTimer += DeltaSeconds;

	AttackSelectedEnemy(targetActor);

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	TArray<FHitResult> HitArray;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitArray, true, FLinearColor::Green, FLinearColor::Red, 0);

	if (Hit)
	{
		for (const FHitResult HitResult : HitArray)
		{
			if (HitResult.GetActor()->ActorHasTag("Enemy"))
			{
				IAttackSystem::Execute_InRangeEnemy(this->GetController(), HitResult.GetActor(), true);
			}
		}
	}

	/*if (Hp == (MaxHp / 2))
	{
		AttackSpeed *= 2;
	}*/

	if (attackTimer >= (1 / AttackSpeed) && isInRangeToAttack == true)
	{
		canAttack = true;
	}
	else
	{
		canAttack = false;
		isAttacking = false;
	}

}

void ARo3otCharacter::AttackSelectedEnemy(AActor* Enemy)
{
	isAttacking = true;
	if (canAttack && Enemy != nullptr)
	{
		IAttackSystem::Execute_AttackEnemy(this, Enemy, AttackDamage, true);
		attackTimer = 0.0f;
		canAttack = false;
	}
}

void ARo3otCharacter::takeDamage(int dmg)
{
	this->Hp -= dmg;
	if (this->Hp <= 0)
	{
		this->Hp = 0;
		dieFromAttack(Hp);
	}
}

void ARo3otCharacter::healHealth(int heal)
{
	this->Hp += heal;
	if (this->Hp >= this->MaxHp)
	{
		this->Hp = this->MaxHp;
	}
}


void ARo3otCharacter::dieFromAttack(int HpLeft)
{
	if (HpLeft == 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdPersonMap"));
	}
}
