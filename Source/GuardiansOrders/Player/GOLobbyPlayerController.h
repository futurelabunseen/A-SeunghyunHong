// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Share/ShareEnums.h"
#include "Interface/GOLobbyHUDInterface.h"
#include "GOLobbyPlayerController.generated.h"

class UGOLobbyHUDWidget;

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API AGOLobbyPlayerController : public APlayerController, public IGOLobbyHUDInterface
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

// HUD Section
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
    TSubclassOf<UGOLobbyHUDWidget> GOLobbyHUDWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
    TObjectPtr<UGOLobbyHUDWidget> GOLobbyHUDWidget;

    virtual void SetupLobbyHUDWidget(UGOLobbyHUDWidget* InLobbyHUDWidget) override;
};
