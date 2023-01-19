// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPS_Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "CubeEnemyPawn.generated.h"

USTRUCT(BlueprintType)
struct FEnemyShootSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireRate = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float MaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf <AFPS_Projectile> ProjectileClass = nullptr;
};


UCLASS()
class RO3OT_API ACubeEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACubeEnemyPawn();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
		void TakeDamage(int dmg);

	void SpawnProjectile(FVector Start, FVector Direction);
	void HitScan(FVector Start, FVector Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FEnemyShootSettings Settings = FEnemyShootSettings();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float InteractionMaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		UArrowComponent* ProjectileArrow;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	int health;
};
