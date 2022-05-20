// Fill out your copyright notice in the Description page of Project Settings.


#include "SHUD.h"
#include "SMainWidget.h"
#include "Blueprint/UserWidget.h"


// --------------------------------------------------------------------
// --------------------------------------------------------------------
ASHUD::ASHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetObject(TEXT("/Game/UI/BP_MainWidget"));
    MainWidgetClass = MainWidgetObject.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> SpecialsWidgetObject(TEXT("/Game/UI/BP_SpecialsButton"));
    SpecialsButtonClass = SpecialsWidgetObject.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> FrameDataRowObject(TEXT("/Game/UI/BP_FrameDataRow"));
    FrameDataRowClass = FrameDataRowObject.Class;
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
ASHUD::~ASHUD()
{
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASHUD::BeginPlay()
{
    Super::BeginPlay();


    if (MainWidgetClass != nullptr)
    {
        MainWidget = CreateWidget<USMainWidget>(GetWorld(), MainWidgetClass);

        if (MainWidget)
        {
            MainWidget->AddToViewport();
            MainWidget->InitializeMainWidget();
        }
    }


    // Set input mode
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(MainWidget->TakeWidget());
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
    }
}
