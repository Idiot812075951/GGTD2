// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGTD2GameMode.h"
#include "GGTD2PlayerController.h"
#include "GGTD2Character.h"
#include "UObject/ConstructorHelpers.h"

AGGTD2GameMode::AGGTD2GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGGTD2PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GGTD/Gameplay/Character/BP_Player"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}