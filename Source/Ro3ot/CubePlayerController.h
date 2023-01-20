// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCube.h"
#include "CubePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RO3OT_API ACubePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACubePlayerController();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void MoveHorizontal(float value);
	void MoveVertical(float value);
	void Shoot();
	
	APlayerCube* Cube = nullptr;
};
