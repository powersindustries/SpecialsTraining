// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGameModeBase.h"
#include "../UI/SHUD.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/SMainWidget.h"
#include "SCharacter.h"


// --------------------------------------------------------------------
// --------------------------------------------------------------------
ASGameModeBase::ASGameModeBase()
{
    HUDClass = ASHUD::StaticClass();
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
ASGameModeBase::~ASGameModeBase()
{
}
