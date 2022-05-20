// Fill out your copyright notice in the Description page of Project Settings.


#include "SSpecialsButton.h"
#include "Components/Button.h"
#include "../Core/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SHUD.h"
#include "SMainWidget.h"


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USSpecialsButton::NativeConstruct()
{
    Super::NativeConstruct();

    MainButton->OnClicked.AddDynamic(this, &USSpecialsButton::OnButtonClicked);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USSpecialsButton::OnButtonClicked()
{
    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        Player->SpecialMove = SpecialMove;
    }

    ASHUD* HUD = Cast<ASHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
    if (HUD)
    {
        HUD->MainWidget->UpdateSpecialUI(SpecialMove); 
    }
}
