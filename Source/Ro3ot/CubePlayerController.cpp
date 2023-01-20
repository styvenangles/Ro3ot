// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerController.h"
#include "Kismet/GameplayStatics.h"

ACubePlayerController::ACubePlayerController()
{
	DefaultMouseCursor = EMouseCursor::Default;
}

void ACubePlayerController::BeginPlay()
{
	Super::BeginPlay();
	Cube = Cast<APlayerCube>(GetPawn());
}

void ACubePlayerController::Tick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);
	
	ACharacter* playerCube= UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector mouseLocation, mouseDirection;
	this->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
	if (playerCube != nullptr)
		Cube = Cast<APlayerCube>(playerCube);
	{
		FRotator currentCharacterRotation = Cube->GetActorRotation();
		FRotator targetRotation = mouseDirection.Rotation();

		FRotator newRotation = FRotator(currentCharacterRotation.Pitch, targetRotation.Yaw, currentCharacterRotation.Roll);
		Cube->SetActorRotation(newRotation);
	}
}

void ACubePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Vertical", this, &ACubePlayerController::MoveVertical);
	InputComponent->BindAxis("Horizontal", this, &ACubePlayerController::MoveHorizontal);

	InputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ACubePlayerController::Shoot);
}

void ACubePlayerController::MoveHorizontal(float value)
{
	if (Cube != nullptr) {
		FVector Direction = Cube->GetActorRightVector();
		Cube->AddMovementInput(Direction, value);
	}
}

void ACubePlayerController::MoveVertical(float value)
{
	if (Cube != nullptr)
	{
		FVector Direction = Cube->GetActorForwardVector();
		Cube->AddMovementInput(Direction, value);
	}
}

void ACubePlayerController::Shoot()
{
	Cube->Shoot();
}
