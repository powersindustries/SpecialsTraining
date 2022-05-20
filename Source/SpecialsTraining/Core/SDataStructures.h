// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "SDataStructures.generated.h"


UENUM()
enum class EButtonState
{
    ePressed,
    eReleased
};


UENUM()
enum class EInputTypes
{
    eNorth,
    eSouth,
    eEast,
    eWest,
    eSubmit
};


UENUM()
enum class ESpecialMove
{
    eQSF,
    eQSB,
    eDP,
    eSuper
};


USTRUCT(BlueprintType)
struct FInputData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    EInputTypes InputType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    EButtonState State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float TimeStamp;
};


USTRUCT(Blueprintable)
struct FSpecialMovesDatatableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText PopularMoves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESpecialMove SpecialMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EInputTypes> DirectionalInputs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int LinksBetweenInputs;

};