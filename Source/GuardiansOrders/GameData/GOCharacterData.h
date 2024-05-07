#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOCharacterStat.h"
#include "Share/ShareEnums.h"
#include "GOCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FGOCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGOCharacterData() :
		HeroType(EHeroType::None),
		HeroIcon(nullptr),
		BaseSkillClass(nullptr),
		SkillQClass(nullptr),
		SkillWClass(nullptr),
		SkillEClass(nullptr),
		SkillRClass(nullptr),
		DefaultBaseSkillName(NAME_None),
		DefaultSkillNameQ(NAME_None),
		DefaultSkillNameW(NAME_None),
		DefaultSkillNameE(NAME_None),
		DefaultSkillNameR(NAME_None),
		SkeletalMesh(nullptr),
		AnimBlueprint(nullptr),
		Spell01Class(nullptr),
		Spell02Class(nullptr),
		Spell03Class(nullptr),
		DefaultSpellName01(NAME_None),
		DefaultSpellName02(NAME_None),
		DefaultSpellName03(NAME_None)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	EHeroType HeroType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TObjectPtr<UTexture2D> HeroIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<class UGOSkillBase> BaseSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<class UGOSkillBase> SkillQClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<UGOSkillBase> SkillWClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<UGOSkillBase> SkillEClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<UGOSkillBase> SkillRClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultBaseSkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSkillNameQ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSkillNameW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSkillNameE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSkillNameR;

	UPROPERTY(EditAnywhere, Category = CharacterData)
	TObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = CharacterData)
	TSubclassOf<class UAnimInstance> AnimBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<class UGOSpellBase> Spell01Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<UGOSpellBase> Spell02Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TSubclassOf<UGOSpellBase> Spell03Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSpellName01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSpellName02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	FName DefaultSpellName03;
};

