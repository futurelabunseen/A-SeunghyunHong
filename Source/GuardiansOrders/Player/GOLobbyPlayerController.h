// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Share/ShareEnums.h"
#include "GOLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API AGOLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AGOLobbyPlayerController();
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSelectHero(EHeroType HeroType);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerReady();

    void EnableMouseCursor();

};
