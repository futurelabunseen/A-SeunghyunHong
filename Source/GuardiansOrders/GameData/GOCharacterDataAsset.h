// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Share/ShareEnums.h"
#include "GOCharacterDataAsset.generated.h"

UCLASS(BlueprintType)
class GUARDIANSORDERS_API UGOCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UGOCharacterDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EHeroType HeroType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	ERoleType RoleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EAttackRange AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EArchetype Archetype;

	UPROPERTY(EditAnywhere, Category = Character)
	TObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = Character)
	TSubclassOf<class UAnimInstance> AnimBlueprint;
};
