// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCube.h"
#include "CubePlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FBooleanDelegate, bool)

/**
 * 
 */
UCLASS()
class RO3OT_API ACubePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACubePlayerController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	APlayerCube* Cube = nullptr;
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void MoveHorizontal(float value);
	void MoveVertical(float value);
	void Shoot(bool TriggerIsPulled);
	
};
