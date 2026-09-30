// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NameInputWidget.h"
#include "Framework/TestPlayerState.h"

#include "Components/EditableTextBox.h"

void UNameInputWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NameInput)
    {
        NameInput->OnTextCommitted.AddDynamic(this, &UNameInputWidget::OnNameInputCommitted);
    }
}

void UNameInputWidget::OnNameInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // 엔터가 아니면 처리 안하고 리턴
    if (CommitMethod != ETextCommit::OnEnter) { return; }

    if (ATestPlayerState* TestPS = GetOwningPlayerState<ATestPlayerState>())
    {
        TestPS->SetMyPlayerName(Text.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UNameInputWidget::OnNameInputCommitted()] : PlayerState가 없습니다."));
    }
}
