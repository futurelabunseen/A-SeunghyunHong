// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Share/EGOSkill.h"
#include "GameData/GOSkillDataAsset.h"
#include "GOSkillBase.generated.h"

class UGOSkillStatComponent;
class UAnimMontage;
class UMaterial;
class UNiagaraSystem;

UCLASS()
class GUARDIANSORDERS_API UGOSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UGOSkillBase();
	virtual void PostInitProperties() override;
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	/**
	  * 구체적인 스킬 내용
	  */
	virtual void Activate() {};

public:
	FORCEINLINE const FString& GetName() const { return Name; }
	FORCEINLINE float GetCoolDownTime() const { return CoolDownTime; }
	FORCEINLINE void SetCooldown() { CoolDownTimer = CoolDownTime; }
	
// Stat Section
protected:


// Data Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	UGOSkillDataAsset* SkillDataAsset;

public:
	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	FString Name;	
	
	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	FText Description;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	EGOSkillType Type;

	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, Category = SkillSetting)
	TObjectPtr<UAnimMontage> SkillAnim;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	TObjectPtr <UMaterial> DecalMaterial;

	UPROPERTY(EditAnywhere, Category = SkillSetting)
	TObjectPtr <UNiagaraSystem> VFX;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float DamageMultiplier = 0;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float DamageRange = 0;
	
	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float DamageRadius = 0;
	
	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float DamageSpeed = 0;	
	
	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float ManaCost = 0;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = SkillSetting)
	float CoolDownTime;

	UPROPERTY(VisibleInstanceOnly, Category = SkillSetting)
	float CoolDownTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "SkillSetting/AutoDetection")
	float DetectionRadius = 0;

	UPROPERTY(EditDefaultsOnly, Category = "SkillSetting/AutoDetection")
	float DetectionDegree = 0;
};