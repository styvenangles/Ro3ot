// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Projectile.h"

// Sets default values
AFPS_Projectile::AFPS_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AFPS_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AFPS_Projectile::OnActorBeginOverlap);

	FVector BaseVelocity = ProjectileDirection * ProjectileSpeed;
	ProjectileComp->SetVelocityInLocalSpace(BaseVelocity);
}

// Called every frame
void AFPS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTimer += DeltaTime;
	if (LifeTimer >= LifeTime)
	{
		LifeTimeEnded();
	}

}

void AFPS_Projectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PSettings.ParentClass = OtherActor->GetClass();

	for (int i = 0; i < PSettings.FriendlyClass.Num(); i++)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("OverlapedActor : %s"), *OtherActor->GetClass()->GetName());*/
		if (PSettings.EnemyClass[i] == OtherActor->GetClass())
		{
			Destroy();
		}
	}
}


void AFPS_Projectile::OnStop(const FHitResult& Hit)
{
	Destroy();
}

void AFPS_Projectile::LifeTimeEnded()
{
	Destroy();
}