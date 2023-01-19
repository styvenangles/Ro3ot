// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPS_Projectile.generated.h"

UCLASS()
class RO3OT_API AFPS_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPS_Projectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector ProjectileDirection = FVector(1, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float ProjectileSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float LifeTime = 2.5f;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnStop(const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UProjectileMovementComponent* ProjectileComp;


private:
	float LifeTimer = 0.0f;

	void LifeTimeEnded();
};
