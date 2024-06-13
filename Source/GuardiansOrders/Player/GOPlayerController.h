// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Share/ShareEnums.h"
#include "Interface/GOPlayerInterface.h"
#include "GOPlayerController.generated.h"

class UGOHUDWidget;
class UGOSkillSetBarWidget;
class IGOHighlightInterface;
class AGOMagicCircle;

UCLASS()
class GUARDIANSORDERS_API AGOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGOPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;
	
	/**
	원격 클라이언트에서 네트워크로 초기화에 필요한 정보를 전달받은 것이 
	모두 마무리되면 (클라이언트에서만) 호출되는 함수입니다.
	*/
	virtual void PostNetInit() override;
	
	virtual void PostSeamlessTravel() override;

	virtual void BeginPlay() override;

	/**
	빙의할 때 발생하는 이벤트 함수입니다.
	*/
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;


protected:
	/**
	 * Sync time between client and server
	 */

	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f; // 5초마다 서버와 동기화

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	float SingleTripTime = 0.f;


	UPROPERTY()
	class AGOGameState* GOBattleGameState;

public:
	void  OnMatchStateSet(FName State);

// HUD Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HUD)
	TSubclassOf<UGOHUDWidget> GOHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=HUD)
	TObjectPtr<UGOHUDWidget> GOHUDWidget;

	void SetHUDScore(float Score);

	void SetHUDDefeats(int32 Defeats);

	void SetHUDMatchCountdown(float CountdownTime);

	void SetHUDTime();

	void AddCharacterOverlayDelayed();

	void HideTeamScores();
	void InitTeamScores();
	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);

	void SetHUDMatchMembers(int32 MatchMemberNum);

	void SetGrindingStoneVisible();

// Magic Circle
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	void HideMagicCircle();

private:
	float MatchTime = 200.f; //200 seconds 
	uint32 CountdownInt = 0;

	FTimerHandle CharacterOverlayTimerHandle;
	FTimerHandle MatchStartTimerHandle; 

	void StartMatchCountdown();
	void CheckMatchState(); // MatchState를 확인하는 함수 선언

// ======== SkillSetBar UI ======== 
protected:
	// SkillSetBar Widget 인스턴스
	UPROPERTY()
	TObjectPtr<UGOSkillSetBarWidget> SkillSetBarWidget;

public:
	void InitializeSkills();


// ======== SpawnAndPossess ======== 
public:
	// New function to spawn and possess a character
	void SpawnAndPossessCharacter();

private:
	// Helper function to get the selected hero type from the player state
	EHeroType GetSelectedHero();

// Mouse Cursor Trace
	void CursorTrace();

	TScriptInterface<IGOHighlightInterface> LastActor;
	TScriptInterface<IGOHighlightInterface> ThisActor;
	FHitResult CursorHit;

// ======= MagicCircle =======
	void UpdateMagicCircleLocation();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGOMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AGOMagicCircle> MagicCircle;

// ======= MatchState  =======
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName  MatchState;

	UFUNCTION()
	void  OnRep_MatchState();
};
