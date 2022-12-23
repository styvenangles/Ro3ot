// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ro3otGameMode.h"
#include "Ro3otCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARo3otGameMode::ARo3otGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
