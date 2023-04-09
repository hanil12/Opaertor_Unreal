// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE_OperatorGameModeBase.h"
#include "MyCharacter.h"

AUE_OperatorGameModeBase::AUE_OperatorGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_MyCharacter(TEXT("Blueprint'/Game/BluePrint/MyCharacter_BP.MyCharacter_BP_C'"));
	if (BP_MyCharacter.Succeeded())
	{
		DefaultPawnClass = BP_MyCharacter.Class;
	}
}
