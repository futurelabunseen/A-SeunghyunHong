#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOCharacterStat.h"
#include "GOCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FGOCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGOCharacterData() :
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
		AnimBlueprint(nullptr)
	{}
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
};

