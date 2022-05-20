// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../UI/SHUD.h"
#include "../UI/SMainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SGameModeBase.h"


// --------------------------------------------------------------------
// --------------------------------------------------------------------
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    static ConstructorHelpers::FObjectFinder<UDataTable> SpecialsDatatableObject(TEXT("/Game/Data/SpecialMovesDatatable"));
    if (SpecialsDatatableObject.Succeeded())
    {
        SpecialsDatatable = SpecialsDatatableObject.Object;
    }

    bAcceptInputs = true;
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("North", IE_Pressed, this, &ASCharacter::OnNorthPressed);

    PlayerInputComponent->BindAction("South", IE_Pressed, this, &ASCharacter::OnSouthPressed);

    PlayerInputComponent->BindAction("East", IE_Pressed, this, &ASCharacter::OnEastPressed);

    PlayerInputComponent->BindAction("West", IE_Pressed, this, &ASCharacter::OnWestPressed);

    PlayerInputComponent->BindAction("Submit", IE_Pressed, this, &ASCharacter::OnSubmitPressed);

}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

    SpecialsDatatable->GetAllRows(TEXT(""), SpecialsData);

    int SpecialsCount = SpecialsData.Num();
    for (int x=0; x < SpecialsCount; ++x)
    {
        FSpecialMovesDatatableRow* CurrentSpecial = SpecialsData[x];
        SpecialsDataMap.Add(CurrentSpecial->SpecialMove, CurrentSpecial);
    }

    SpecialMove = SpecialsData[0]->SpecialMove;
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::OnNorthPressed()
{
    ManageNewInputs(EInputTypes::eNorth, EButtonState::ePressed, GetWorld()->GetTimeSeconds());
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::OnSouthPressed()
{
    ManageNewInputs(EInputTypes::eSouth, EButtonState::ePressed, GetWorld()->GetTimeSeconds());
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::OnEastPressed()
{
    ManageNewInputs(EInputTypes::eEast, EButtonState::ePressed, GetWorld()->GetTimeSeconds());
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::OnWestPressed()
{
    ManageNewInputs(EInputTypes::eWest, EButtonState::ePressed, GetWorld()->GetTimeSeconds());
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::OnSubmitPressed()
{
    ManageNewInputs(EInputTypes::eSubmit, EButtonState::ePressed, GetWorld()->GetTimeSeconds());
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
bool ASCharacter::SpecialCorrectAndUpdateFrameDataUI(const ESpecialMove& Special)
{
    FSpecialMovesDatatableRow* data = SpecialsDataMap[Special];
    TArray<EInputTypes> MotionInputArray = data->DirectionalInputs;

    int InputsSuceeded = 0;
    int SpecialMoveIndex = 0;
    int InputBufferSize = InputBuffer.Num();
    if (InputBufferSize > 0)
    {
        for (int x = 0; x < InputBufferSize; ++x)
        {
            FInputData* CurrentInputData = &InputBuffer[x];

            if (MotionInputArray[SpecialMoveIndex] == CurrentInputData->InputType)
            {
                if (SpecialMoveIndex == 0)
                {
                    InputsSuceeded++;
                    SpecialMoveIndex++;
                }
                else
                {
                    FInputData* LastInput = &InputBuffer[x - 1];
                    float TimeBetweenInputs = LastInput->TimeStamp - CurrentInputData->TimeStamp;
                    if (TimeBetweenInputs <= (FrameLengthInSeconds * data->LinksBetweenInputs))
                    {
                        InputsSuceeded++;
                        SpecialMoveIndex++;
                    }
                }

                if (InputsSuceeded == MotionInputArray.Num())
                {
                    return true;
                }

            }
        }
    }

    return InputsSuceeded == MotionInputArray.Num();
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void ASCharacter::ManageNewInputs(EInputTypes Input, EButtonState State, float TimeStamp)
{
    if (!bAcceptInputs)
    {
        return;
    }

    if (State == EButtonState::ePressed)
    {
        FInputData InputData;
        InputData.InputType = Input;
        InputData.State     = State;
        InputData.TimeStamp = TimeStamp;
        InputBuffer.Add(InputData);

        ASHUD* MainHUD = Cast<ASHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
        if (MainHUD)
        {
            MainHUD->MainWidget->DisplayNewInput(InputData.InputType);
        }
    }
}
