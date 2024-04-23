#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOSkillStat.generated.h"

USTRUCT(BlueprintType)
struct FGOSkillStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGOSkillStat() :
		DamageMultiplier(0.0f),
		DamageRange(0.0f),
		DamageRadius(0.0f),
		DamageSpeed(0.0f),
		CoolDownTime(0.f),
		CastingTime(0.f),
		ManaCost(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float DamageSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float CoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float CastingTime; // CastDuratoinTime // ex. AttackTime

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillStat)
	float ManaCost;

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