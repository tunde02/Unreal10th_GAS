// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerState.h"

#include "Net/UnrealNetwork.h"

void ATestPlayerState::AddMyPlayerScore(int32 InPoint)
{
    if (HasAuthority())
    {
        MyPlayerScore += InPoint;
        OnRepNotify_MyPlayerScore();
    }
}

void ATestPlayerState::SetMyPlayerName(const FString& InNewName)
{
    if (HasAuthority())
    {
        if (InNewName.IsEmpty())
        {
            MyPlayerName = TEXT("플레이어");
        }
        else
        {
            MyPlayerName = InNewName;
        }

        OnRepNotify_MyPlayerName();
    }
    else
    {
        // 클라이언트가 서버에 PlayerName 바꿔달라고 요청
        Server_SetMyPlayerName(InNewName);
    }
}

void ATestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATestPlayerState, MyPlayerScore);
    DOREPLIFETIME(ATestPlayerState, MyPlayerName);
}

bool ATestPlayerState::Server_SetMyPlayerName_Validate(const FString& InNewName)
{
    return InNewName.Len() <= 10;
}

void ATestPlayerState::Server_SetMyPlayerName_Implementation(const FString& InNewName)
{
    SetMyPlayerName(InNewName);
}

void ATestPlayerState::OnRepNotify_MyPlayerScore()
{
    OnScoreChanged.Broadcast(MyPlayerScore);
}

void ATestPlayerState::OnRepNotify_MyPlayerName()
{
    OnNameChanged.Broadcast(MyPlayerName);
}
