﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/GOSkillBase.h"
#include "GameData/GOGameSubsystem.h"
#include "GOSkillCastComponent.generated.h"

class UGOProjectileSkillBase;
class AGOProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOSkillCastComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOSkillCastComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	  * 스킬이 시작될 때 발생하는 이벤트를 처리합니다.
	  */
	void OnStartCast(FHeroSkillKey Key);
	
	/**
	  * 스킬이 활성화되는 동안 계속해서 발생하는 이벤트를 처리합니다.
	  */
	void OnUpdateCast(float DeltaTime);

	/**
	  * 스킬 사용이 완료되었을 때 발생하는 이벤트를 처리합니다.
	  */
	void OnFinishCast();

	/**
	  * 스킬이 중단될 때 발생하는 이벤트를 처리합니다.
	  */
	void OnInterruptCast();

	void UpdateCoolDownTime(float DeltaTime);

public:
	// 현재 스킬 Get, Set
	void SetCurrentSkillByKey(FHeroSkillKey Key);
	void SetCurrentSkillKey(FHeroSkillKey Key);
	TObjectPtr<UGOSkillBase> GetCurrentSkill();
	FHeroSkillKey GetCurrentSkillKey();

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void HandleProjectileSkill(UGOProjectileSkillBase* ProjectileSkill, FVector Location, FRotator Rotation, FTransform SpawnTransform);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHandleProjectileSkill(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, FRotator Rotation, FTransform SpawnTransform);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnProjectile(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, FRotator Rotation, FTransform SpawnTransform);

private:
	//  현재 캐스팅 중인지의 여부 
	bool bIsOnCasting = false;

	TObjectPtr<UGOSkillBase> CurrentSkill;
	FHeroSkillKey SkillKey;
	float CastDownTimer = 0.f;
};

/*
EGOSkillType에 따라 다르게 처리
*/