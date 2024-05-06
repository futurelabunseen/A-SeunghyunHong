#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOSpellStat.generated.h"

USTRUCT(BlueprintType)
struct FGOSpellStat : public FTableRowBase // Row 이름을 뒤에 만들고... 커스텀 구조체를 보통FGOSkillStat 이런식으로 
{
	GENERATED_BODY()

public:
	FGOSpellStat() :
		DamageMultiplier(0.0f),
		DamageRange(0.0f),
		DamageRadius(0.0f),
		DamageSpeed(0.0f),
		CoolDownTime(0.f),
		MoveSpeedMultiplier(0.0f),
		HealAmount(0.0f)
		{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float DamageSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float CoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float MoveSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellStat)
	float HealAmount;

	//FGOSkillStat operator+(const FGOSkillStat& Other) const
	//{
	//	const float* const ThisPtr = reinterpret_cast<const float* const>(this);
	//	const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

	//	FGOSkillStat Result;
	//	float* ResultPtr = reinterpret_cast<float*>(&Result);
	//	int32 StatNum = sizeof(FGOSkillStat) / sizeof(float);
	//	for (int32 i = 0; i < StatNum; i++)
	//	{
	//		ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
	//	}

	//	return Result;
	//}
};