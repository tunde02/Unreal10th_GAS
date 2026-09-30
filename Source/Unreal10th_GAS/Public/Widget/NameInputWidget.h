// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NameInputWidget.generated.h"

class UEditableTextBox;

UCLASS()
class UNREAL10TH_GAS_API UNameInputWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnNameInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UEditableTextBox> NameInput;

};
