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
		SkillQ(nullptr),
		SkillW(nullptr),
		SkillE(nullptr),
		SkillR(nullptr),
		DefaultBaseSkillName(NAME_None),
		DefaultSkillNameQ(NAME_None),
		DefaultSkillNameW(NAME_None),
		DefaultSkillNameE(NAME_None),
		DefaultSkillNameR(NAME_None),
		SkeletalMesh(nullptr),
		AnimBlueprint(nullptr)
		{}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TObjectPtr<class UGOSkillBase> SkillQ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TObjectPtr<UGOSkillBase> SkillW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TObjectPtr<UGOSkillBase> SkillE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterData)
	TObjectPtr<UGOSkillBase> SkillR;

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

