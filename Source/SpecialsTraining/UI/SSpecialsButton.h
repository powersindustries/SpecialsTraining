// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Core/SDataStructures.h"
#include "SSpecialsButton.generated.h"

UCLASS()
class SPECIALSTRAINING_API USSpecialsButton : public UUserWidget
{
	GENERATED_BODY()


public:	

    void NativeConstruct() override;

    UFUNCTION()
    void OnButtonClicked();


public:	

    UPROPERTY(meta = (BindWidget))
    class UButton* MainButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LabelText;

    ESpecialMove SpecialMove;


};
