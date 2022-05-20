// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Core/SDataStructures.h"
#include "SMainWidget.generated.h"

UCLASS()
class SPECIALSTRAINING_API USMainWidget : public UUserWidget
{
	GENERATED_BODY()


public:

    void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void InitializeMainWidget();
    void DisplayNewInput(EInputTypes Input);
    void UpdateSpecialUI(ESpecialMove& Special);

    UFUNCTION()
    void OnClear();

    UFUNCTION()
    void OnStartSpecialButton();

    UFUNCTION()
    void OnCountdownCompleted();

    UFUNCTION()
    void OnInputCompleted();


private:
    
    FText InputTypeToText(const EInputTypes InputType);

    void ClearAllUI();
    void SetEnabledForSpecialsButtons(bool bEnabled);
    void DisplaySuccessMissUI(bool InputCorrect);


public:

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* InputScrollbox;

    UPROPERTY(meta = (BindWidget))
    class UButton* ClearInputsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartSpecialButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* SpecialsScrollbox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* FrameDataScrollbox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SpecialNameTextblock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SpecialDescriptionTextblock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CountdownTextblock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PopularMovesTextblock;

    UPROPERTY(meta = (BindWidget))
    class UBorder* SuccessBox;

    UPROPERTY(meta = (BindWidget))
    class UBorder* FailureBox;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* CountdownBar;


private:    

    const uint8 MAX_FPS = 60;

    const float FrameLengthInSeconds = 0.0166f;
    const float CountdownTime        = 4.0f;
    const float InputCountdownTime   = 1.0f;

    float BaseStartingTime = 0.0f;

    bool bShowCountdownTimer = false;
    bool bShowInputTimer     = false;

    FTimerHandle CountdownTimerHandle;
    FTimerHandle InputTimerHandle;

    float DisplayCountDownTracker;
    float InputCountDownTracker;


};
