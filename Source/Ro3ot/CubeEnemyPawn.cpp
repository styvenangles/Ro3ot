// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeEnemyPawn.h"

// Sets default values
ACubeEnemyPawn::ACubeEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 50;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileLocation = CreateDefaultSubobject<USceneComponent>("ProjectileLocation");
	ProjectileLocation->SetupAttachment(ProjectileArrow);
}

// Called when the game starts or when spawned
void ACubeEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACubeEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACubeEnemyPawn::Shoot()
{
	FVector Direction = this->GetActorForwardVector();
	FVector Start = this->GetActorLocation() + (Direction * 100.0f);

	if (Settings.ProjectileClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy shooting!"));
		SpawnProjectile(Direction);
	}
	else
	{
		HitScan(Start, Direction);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Direction.ToString());
}

void ACubeEnemyPawn::HitScan(FVector Start, FVector Direction)
{
	FHitResult Hit = FHitResult();

	FVector End = Start + (Direction * Settings.MaxDistance);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FVector HitLocation = Hit.bBlockingHit ? Hit.Location : End;

	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 8, FColor::Red, false, 1.f);

}

void ACubeEnemyPawn::SpawnProjectile(FVector Direction)
{
	FActorSpawnParameters Params;

	FVector Location = ProjectileLocation->GetComponentLocation();
	FRotator Rotation = Direction.Rotation();

	GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);
}

void ACubeEnemyPawn::TakeDamage(int dmg)
{
	health -= dmg;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Life: %i"), health);
	if (health <= 0)
	{
		Destroy();
	}
}