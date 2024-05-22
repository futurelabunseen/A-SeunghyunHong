// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Share/ShareEnums.h"
#include "GOLobbyGameMode.generated.h"

class UGOCheatManager; 

UCLASS()
class GUARDIANSORDERS_API AGOLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGOLobbyGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	//void SetSelectedCharacter(TSubclassOf<class AGOPlayerCharacter> CharacterClass);
	void SelectHero(APlayerController* PlayerController, EHeroType HeroType);

	UFUNCTION()
	void OnGamePlayerReady();

	// 선택된 캐릭터 정보를 저장하는 변수
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TMap<APlayerController*, TSubclassOf<class AGOPlayerCharacter>> PlayerCharacterClasses;

	UPROPERTY(EditAnywhere, Category = "Character Classes")
	TSubclassOf<class AGOPlayerCharacter> RogersCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Character Classes")
	TSubclassOf<AGOPlayerCharacter> KatnissCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Character Classes")
	TSubclassOf<AGOPlayerCharacter> BeastCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Character Classes")
	TSubclassOf<AGOPlayerCharacter> BrideCharacterClass;

private:
	//TMap<APlayerController*, EHeroType> PlayerCharacterSelection;

	void CheckAllPlayersSelected();
	void SetupInputMode(APlayerController* PlayerController);
	void SpawnPlayerCharacter(APlayerController* NewPlayer, EHeroType HeroType);

	
};
