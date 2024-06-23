// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GOCharacterMovementComponent.generated.h"


class FGOSavedMove_Character : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

public:
	virtual void Clear() override;
	virtual void SetInitialPosition(ACharacter* Character) override;
	virtual uint8 GetCompressedFlags() const override;

	uint8 bPressedFlashSpell : 1;
	uint8 bDidFlash : 1;

	uint8 bPressedGhostSpell : 1;
	uint8 bDidGhost : 1;
};



// 클라이언트 캐릭터 데이터를 관리할 클래스입니다.
class FGONetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
	typedef FNetworkPredictionData_Client_Character Super;

public:
	FGONetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
	virtual FSavedMovePtr AllocateNewMove() override;
};

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UGOCharacterMovementComponent();

	void SetFlashSpellCommand();

	void SetGhostSpellCommand();

protected:
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	/**
	 * 실제로 점멸을 진행하는 함수
	 */
	virtual void GOFlash();

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	/**
	 * 유체화 
	 */
	virtual void GOGhost();

	bool CanTeleport(FVector& TargetLocation) const; // 추가
	bool IsInvisibleWall(const FHitResult& HitResult) const;

public:
	uint8 bPressedFlashSpell : 1; // 입력이 들어오면 참으로 변경 예정

	/**
	 * 네트워크로 전달될 수도 있으므로 uint8 타입
	 * 점멸 상태를 나타내는 플래그 변수들
	 * 점멸이 실행되서 쿨타임이 지날 때까지는 모두 True로 설정되고,
	 * 다시 점멸을 시작할 수 있을 때 False로 설정할 것입니다.
	 */
	uint8 bDidFlash : 1;

	uint8 bPressedGhostSpell : 1;
	uint8 bDidGhost : 1;

protected:
	/**
	 * 점멸 거리
	 */
	UPROPERTY()
	float FlashMovementOffset;

	/**
	 * 점멸 쿨타임
	 */
	UPROPERTY()
	float FlashCoolTime;

	/**
	 * 유체화 계수
	 */
	UPROPERTY()
	float GhostSpeedMultiplier;

	/**
	 * 유체화 쿨타임
	 */
	UPROPERTY()
	float GhostCoolTime;
};
