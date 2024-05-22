#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOHeroCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FGOHeroCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGOHeroCharacterData () :

		RogersCharacterClass(nullptr),
		KatnissCharacterClass(nullptr),
		BeastCharacterClass(nullptr),
		BrideCharacterClass(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Classes")
	TSubclassOf<class AGOPlayerCharacter> RogersCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Classes")
	TSubclassOf<class AGOPlayerCharacter> KatnissCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Classes")
	TSubclassOf<class AGOPlayerCharacter> BeastCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Classes")
	TSubclassOf<class AGOPlayerCharacter> BrideCharacterClass;

};

