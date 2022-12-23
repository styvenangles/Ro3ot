// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultClassEnemy.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ADefaultClassEnemy::ADefaultClassEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADefaultClassEnemy::BeginPlay()
{
	Super::BeginPlay();

	Hp = MaxHp;
}

// Called every frame
void ADefaultClassEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	attackTimer += DeltaTime;

	AttackSelectedEnemy(targetActor);

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	TArray<FHitResult> HitArray;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitArray, true, FLinearColor::Green, FLinearColor::Red, 0);


	// You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
	DrawDebugSphere(GetWorld(), TraceStart, TraceRadius, 18.0f, FColor::Green, false, -1, 0, 1);

	if (Hit)
	{
		for (const FHitResult HitResult : HitArray)
		{
			IAttackSystem::Execute_InRangeEnemy(this, HitResult.GetActor(), false);
		}
	}


	if (attackTimer >= (1 / AttackSpeed) && isInRangeToAttack == true)
	{
		canAttack = true;
	}
	else
	{
		canAttack = false;
	}
}

void ADefaultClassEnemy::AttackSelectedEnemy(AActor* Enemy)
{
	if (canAttack && Enemy != nullptr)
	{
		IAttackSystem::Execute_AttackEnemy(this, Enemy, AttackDamage, true);
		attackTimer = 0.0f;
		canAttack = false;
	}
}

void ADefaultClassEnemy::takeDamage(int dmg)
{
	this->Hp -= dmg;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ouch!"));
	if (this->Hp <= 0)
	{
		this->Hp = 0;
		dieFromAttack(Hp);
	}
}

void ADefaultClassEnemy::healHealth(int heal)
{
	this->Hp += heal;
	if (this->Hp >= this->MaxHp)
	{
		this->Hp = this->MaxHp;
	}
}

void ADefaultClassEnemy::dieFromAttack(int HpLeft)
{
	if (HpLeft == 0)
	{
		// Find character in world
		ACharacter* robotCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		robotPC = Cast<ARo3otCharacter>(robotCharacter);
		if (robotPC->Hp < robotPC->MaxHp)
		{
			robotPC->Hp += 10;
		}
		robotPC->targetActor = nullptr;
		robotPC->canAttack = false;
		Destroy();
	}
}
