// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFrameDataRowWidget.generated.h"

UCLASS()
class SPECIALSTRAINING_API USFrameDataRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UBorder* BackgroundBorder;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FrameIndexText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* InputText;

};
