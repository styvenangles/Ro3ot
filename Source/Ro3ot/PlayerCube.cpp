// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"

// Sets default values
APlayerCube::APlayerCube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

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

	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileLocation = CreateDefaultSubobject<USceneComponent>("ProjectileLocation");
	ProjectileLocation->SetupAttachment(ProjectileArrow);
}

// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCube::SpawnProjectile(FVector Start, FVector Direction)
{
	FActorSpawnParameters Params;
	FVector Location = ProjectileLocation->GetComponentLocation() + FVector(100.f, 0.f, 0.f);
	FRotator Rotation = Direction.Rotation();

	GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);
}

void APlayerCube::Shoot()
{
	FVector Direction = this->GetActorForwardVector();
	FVector Start = this->GetActorLocation() + (Direction * 100.0f);
	FVector End = Start + Direction * InteractionMaxDistance;

	if (IsValid(ProjectileLocation))
	{
		SpawnProjectile(Start, Direction);
	}
	else
	{
		HitScan(Start, Direction);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetActorLocation().ToString());
}

void APlayerCube::HitScan(FVector Start, FVector Direction)
{
	FHitResult Hit = FHitResult();

	FVector End = Start + (Direction * Settings.MaxDistance);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FVector HitLocation = Hit.bBlockingHit ? Hit.Location : End;

	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 8, FColor::Red, false, 1.f);

}


