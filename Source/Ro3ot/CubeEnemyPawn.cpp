// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeEnemyPawn.h"

// Sets default values
ACubeEnemyPawn::ACubeEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 3;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->OnComponentHit.AddDynamic(this, &ACubeEnemyPawn::OnHit);

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
	/*FVector Direction = this->GetActorForwardVector();
	FVector Start = this->GetActorLocation() + (Direction * 100.0f);

	if (Settings.ProjectileClass != nullptr)
	{
		SpawnProjectile(Direction);
	}
	else
	{
		HitScan(Start, Direction);
	}*/
}

void ACubeEnemyPawn::HitScan(FVector Start, FVector Direction)
{
	/*FHitResult Hit = FHitResult();

	FVector End = Start + (Direction * Settings.MaxDistance);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FVector HitLocation = Hit.bBlockingHit ? Hit.Location : End;

	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 8, FColor::Red, false, 1.f);*/

}

void ACubeEnemyPawn::SpawnProjectile(FVector Direction)
{
	/*FActorSpawnParameters Params;

	FVector Location = ProjectileLocation->GetComponentLocation();
	FRotator Rotation = Direction.Rotation();

	GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);*/
}

void ACubeEnemyPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*if(OtherActor->GetClass() == Settings.ProjectileClass)
	{
		AFPS_Projectile* projectileInstance = Cast<AFPS_Projectile>(OtherActor);
		SubDamage(projectileInstance->Damage);
	}*/
}


void ACubeEnemyPawn::SubDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		Destroy();
	}
}