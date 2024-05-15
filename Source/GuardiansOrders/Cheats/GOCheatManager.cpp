// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/GOCheatManager.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void UGOCheatManager::ShowDebugLog()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Debug Log Activated"));
        UE_LOG(LogTemp, Warning, TEXT("Debug Log Activated"));
    }
}

void UGOCheatManager::AuthorityStatus()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Authority Status Checked"));
        UE_LOG(LogTemp, Warning, TEXT("Authority Status Checked"));
    }
}
