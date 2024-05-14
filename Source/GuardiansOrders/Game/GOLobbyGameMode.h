// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Share/ShareEnums.h"
#include "GOLobbyGameMode.generated.h"


UCLASS()
class GUARDIANSORDERS_API AGOLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGOLobbyGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSelectedCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCommonUserWidget> HeroSelectionWidgetClass;

private:
	TMap<APlayerController*, EHeroType> PlayerCharacterSelection;

	void CheckAllPlayersSelected();
	void SetupInputMode(APlayerController* PlayerController);

};
