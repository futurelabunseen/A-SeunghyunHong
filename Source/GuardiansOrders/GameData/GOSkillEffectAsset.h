// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GOSkillEffectAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOSkillEffectAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UGOSkillEffectAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillEffect)
	TObjectPtr<UNiagaraSystem> VFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillEffect)
	TObjectPtr<UMaterial> DecalMaterial;
};