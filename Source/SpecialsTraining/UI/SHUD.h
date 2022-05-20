// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SHUD.generated.h"

UCLASS()
class SPECIALSTRAINING_API ASHUD : public AHUD
{
	GENERATED_BODY()


public:

    ASHUD();
    ~ASHUD();
	
	virtual void BeginPlay() override;


public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> MainWidgetClass;
    class USMainWidget* MainWidget;


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> SpecialsButtonClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> FrameDataRowClass;
	
};
