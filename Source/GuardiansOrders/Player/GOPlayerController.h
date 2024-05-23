// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Share/ShareEnums.h"
#include "GOPlayerController.generated.h"

class UGOHUDWidget;
class UGOSkillSetBarWidget;

UCLASS()
class GUARDIANSORDERS_API AGOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGOPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	
	/**
	���� Ŭ���̾�Ʈ���� ��Ʈ��ũ�� �ʱ�ȭ�� �ʿ��� ������ ���޹��� ���� 
	��� �������Ǹ� (Ŭ���̾�Ʈ������) ȣ��Ǵ� �Լ��Դϴ�.
	*/
	virtual void PostNetInit() override;
	
	virtual void PostSeamlessTravel() override;

	virtual void BeginPlay() override;

	/**
	������ �� �߻��ϴ� �̺�Ʈ �Լ��Դϴ�.
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
	float TimeSyncFrequency = 5.f; // 5�ʸ��� ������ ����ȭ

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	float SingleTripTime = 0.f;


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

private:
	float MatchTime = 120.f; //120 seconds 
	uint32 CountdownInt = 0;

	FTimerHandle CharacterOverlayTimerHandle;

// ======== SkillSetBar UI ======== 
protected:
	// SkillSetBar Widget �ν��Ͻ�
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

};
