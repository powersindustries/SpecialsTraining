// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SDataStructures.h"
#include "SCharacter.generated.h"

UCLASS()
class SPECIALSTRAINING_API ASCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	ASCharacter();


public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

    void OnNorthPressed();
    void OnSouthPressed();
    void OnEastPressed();
    void OnWestPressed();
    void OnSubmitPressed();

    bool SpecialCorrectAndUpdateFrameDataUI(const ESpecialMove& Special);


private:

    void ManageNewInputs(EInputTypes Input, EButtonState State, float TimeStamp);


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TArray<FInputData> InputBuffer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
    class UDataTable* SpecialsDatatable;


public:

    bool bAcceptInputs;

	ESpecialMove SpecialMove;

    TArray<FSpecialMovesDatatableRow*> SpecialsData;
    TMap<ESpecialMove, FSpecialMovesDatatableRow*> SpecialsDataMap;


private:

    const float FrameLengthInSeconds = 0.0166f;


};
