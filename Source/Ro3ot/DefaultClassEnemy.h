// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSystem.h"
#include "Ro3otCharacter.h"
#include "DefaultClassEnemy.generated.h"

UCLASS()
class RO3OT_API ADefaultClassEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultClassEnemy();

	UPROPERTY(EditAnywhere, Category = "Collision")
		TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		int Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float AttackDamage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float MouvementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool isMovingToAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool canAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool isInRangeToAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		AActor* targetActor;

	UFUNCTION(BlueprintCallable, Category = "Life")
		void takeDamage(int dmg);

	UFUNCTION(BlueprintCallable, Category = "Life")
		void healHealth(int heal);

	UFUNCTION(BlueprintCallable, Category = "Life")
		void dieFromAttack(int HpLeft);

	UFUNCTION()
		void AttackSelectedEnemy(AActor* Enemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	float attackTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Actor")
		ARo3otCharacter* robotPC = nullptr;

};
