// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "../Core/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "../Core/SGameModeBase.h"
#include "SSpecialsButton.h"
#include "SHUD.h"
#include "Components/VerticalBoxSlot.h"
#include "SFrameDataRowWidget.h"


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ClearInputsButton->OnClicked.AddDynamic(this, &USMainWidget::OnClear);
    StartSpecialButton->OnClicked.AddDynamic(this, &USMainWidget::OnStartSpecialButton);

    DisplayCountDownTracker = CountdownTime;

    CountdownBar->SetVisibility(ESlateVisibility::Collapsed);

    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        UpdateSpecialUI(Player->SpecialMove);
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bShowCountdownTimer)
    {
        CountdownTextblock->SetVisibility(ESlateVisibility::Visible);
        CountdownTextblock->SetText(FText::FromString(FString::FromInt(DisplayCountDownTracker) + " s"));

        DisplayCountDownTracker -= FrameLengthInSeconds;
    }
    else
    {
        CountdownTextblock->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (bShowInputTimer)
    {
        InputCountDownTracker += FrameLengthInSeconds;

        CountdownBar->SetPercent(InputCountDownTracker / InputCountdownTime);
    }
    else
    {
        CountdownBar->SetVisibility(ESlateVisibility::Collapsed);
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::DisplayNewInput(EInputTypes Input)
{
    UTextBlock* InputTextblock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

    InputTextblock->SetText(InputTypeToText(Input));

    if (InputTextblock && !InputTextblock->GetText().IsEmpty())
    {
        UScrollBoxSlot* NewInputSlot = Cast<UScrollBoxSlot>(InputScrollbox->AddChild(InputTextblock));
        NewInputSlot->SetPadding(FMargin(10));

        InputScrollbox->ScrollToEnd();
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::InitializeMainWidget()
{
    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        ASHUD* HUD = Cast<ASHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
        if (HUD)
        {
            int SpecialsSize = Player->SpecialsData.Num();
            for (int x = 0; x < SpecialsSize; ++x)
            {
                FSpecialMovesDatatableRow* CurrentSpecialsData = Player->SpecialsData[x];

                USSpecialsButton* NewSpecialButton = CreateWidget<USSpecialsButton>(this, HUD->SpecialsButtonClass);
                NewSpecialButton->SpecialMove = CurrentSpecialsData->SpecialMove;
                NewSpecialButton->LabelText->SetText(CurrentSpecialsData->Name);
                NewSpecialButton->SetPadding(FVector4(10.0f,10.0f,10.0f,10.0f));
                SpecialsScrollbox->AddChild(NewSpecialButton);
            }
        }
    }

    SuccessBox->SetVisibility(ESlateVisibility::Collapsed);
    FailureBox->SetVisibility(ESlateVisibility::Collapsed);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::UpdateSpecialUI(ESpecialMove& Special)
{
    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        FSpecialMovesDatatableRow* SelectedSpecial = Player->SpecialsDataMap[Special];
        SpecialNameTextblock->SetText(SelectedSpecial->Name);
        SpecialDescriptionTextblock->SetText(SelectedSpecial->Description);
        PopularMovesTextblock->SetText(SelectedSpecial->PopularMoves);
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::OnClear()
{
    ClearAllUI();
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::OnStartSpecialButton()
{
    bShowCountdownTimer = true;

    ClearAllUI();

    StartSpecialButton->SetIsEnabled(false);
    ClearInputsButton->SetIsEnabled(false);
    SetEnabledForSpecialsButtons(false);

    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        Player->bAcceptInputs = false;
    }

    GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &USMainWidget::OnCountdownCompleted, CountdownTime, false);

}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::OnCountdownCompleted()
{
    bShowCountdownTimer = false;
    DisplayCountDownTracker = CountdownTime;

    bShowInputTimer = true;
    InputCountDownTracker = 0;

    CountdownBar->SetVisibility(ESlateVisibility::Visible);

    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        Player->bAcceptInputs = true;
        Player->InputBuffer.Empty();
    }

    BaseStartingTime = GetWorld()->GetTimeSeconds();
    GetWorld()->GetTimerManager().SetTimer(InputTimerHandle, this, &USMainWidget::OnInputCompleted, InputCountdownTime, false);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::OnInputCompleted()
{
    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        DisplaySuccessMissUI(Player->SpecialCorrectAndUpdateFrameDataUI(Player->SpecialMove));

        ASHUD* HUD = Cast<ASHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
        if (HUD)
        {
            for (int x = 0; x < MAX_FPS; ++x)
            {
                USFrameDataRowWidget* NewRow = CreateWidget<USFrameDataRowWidget>(this, HUD->FrameDataRowClass);
                NewRow->FrameIndexText->SetText(FText::AsNumber(x + 1));
                NewRow->SetPadding(FVector4(2.0f,2.0f,2.0f,2.0f));
                NewRow->BackgroundBorder->SetBrushColor(FLinearColor::Gray);
                FrameDataScrollbox->AddChild(NewRow);
            }

            TArray<EInputTypes> MotionInputArray = Player->SpecialsDataMap[Player->SpecialMove]->DirectionalInputs;
            int SpecialMoveIndex = 0;
            int InputBufferSize = Player->InputBuffer.Num();
            if (InputBufferSize > 0)
            {
                for (int x = 0; x < InputBufferSize; ++x)
                {
                    FInputData* CurrentInputData = &Player->InputBuffer[x];

                    int FrameDataIndex = floor((CurrentInputData->TimeStamp - BaseStartingTime) / FrameLengthInSeconds);
                    USFrameDataRowWidget* CurrentDataRowWidget = Cast<USFrameDataRowWidget>(FrameDataScrollbox->GetChildAt(FrameDataIndex));
                    CurrentDataRowWidget->InputText->SetText(InputTypeToText(CurrentInputData->InputType));

                    if (SpecialMoveIndex < MotionInputArray.Num() && MotionInputArray[SpecialMoveIndex] == CurrentInputData->InputType)
                    {
                        CurrentDataRowWidget->BackgroundBorder->SetBrushColor(FLinearColor::Green);
                        SpecialMoveIndex++;
                    }
                    else
                    {
                        CurrentDataRowWidget->BackgroundBorder->SetBrushColor(FLinearColor::Red);
                    }

                    // Scroll to first child
                    if (x == 0)
                    {
                        FrameDataScrollbox->ScrollWidgetIntoView(CurrentDataRowWidget, true, EDescendantScrollDestination::TopOrLeft, 10.0f);
                    }
                }
            }
        }

        Player->InputBuffer.Empty();
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::DisplaySuccessMissUI(bool InputCorrect)
{
    StartSpecialButton->SetIsEnabled(true);
    ClearInputsButton->SetIsEnabled(true);
    SetEnabledForSpecialsButtons(true);

    CountdownBar->SetVisibility(ESlateVisibility::Collapsed);

    SuccessBox->SetVisibility(InputCorrect ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    FailureBox->SetVisibility(!InputCorrect ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::SetEnabledForSpecialsButtons(bool bEnabled)
{
    int ButtonCount = SpecialsScrollbox->GetAllChildren().Num();
    for (int x=0; x < ButtonCount; ++x)
    {
        USSpecialsButton* CurrentButton = Cast<USSpecialsButton>(SpecialsScrollbox->GetChildAt(x));
        CurrentButton->SetIsEnabled(bEnabled);
    }
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
void USMainWidget::ClearAllUI()
{
    InputScrollbox->ClearChildren();

    ASCharacter* Player = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        Player->InputBuffer.Empty();
    }

    FrameDataScrollbox->ClearChildren();

    SuccessBox->SetVisibility(ESlateVisibility::Collapsed);
    FailureBox->SetVisibility(ESlateVisibility::Collapsed);
}


// --------------------------------------------------------------------
// --------------------------------------------------------------------
FText USMainWidget::InputTypeToText(const EInputTypes InputType)
{
    switch (InputType)
    {
    case EInputTypes::eNorth:
    {
        return FText::FromString("Up");
    }
    case EInputTypes::eSouth:
    {
        return FText::FromString("Down");
    }
    case EInputTypes::eEast:
    {
        return FText::FromString("Right");
    }
    case EInputTypes::eWest:
    {
        return FText::FromString("Left");
    }
    case EInputTypes::eSubmit:
    {
        return FText::FromString("Submit");
    }
    default:
        return FText::FromString("NONE");
    }
}
