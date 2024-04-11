// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Share/EGOSkill.h"
#include "Share/ShareEnums.h"
#include "GOSkillDataAsset.generated.h"

class UNiagaraSystem;

UCLASS(BlueprintType)
class GUARDIANSORDERS_API UGOSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UGOSkillDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	EHeroType HeroType; // 우선 넣어보았습니다

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	EGOSkillCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	EGOSkillType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TObjectPtr<UAnimMontage> SkillAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TObjectPtr<UMaterial> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TObjectPtr<UNiagaraSystem> VFX;
};
