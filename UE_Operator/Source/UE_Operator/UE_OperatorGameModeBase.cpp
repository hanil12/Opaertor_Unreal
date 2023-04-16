// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE_OperatorGameModeBase.h"
#include "MyPlayerCharacter.h"

AUE_OperatorGameModeBase::AUE_OperatorGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AMyPlayerCharacter> BP_MyCharacter(TEXT("Blueprint'/Game/BluePrint/Player_BP.Player_BP_C'"));
	if (BP_MyCharacter.Succeeded())
	{
		DefaultPawnClass = BP_MyCharacter.Class;
	}
}
