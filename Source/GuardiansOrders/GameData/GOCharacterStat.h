#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GOSkillStat.h"
#include "GOCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FGOCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGOCharacterStat() :
		MaxHp(0.0f),
		HpRegenerationRate(0.0f),
		MovementSpeed(0.0f),
		MaxBasicAttackRange(0.0f),
		BaseDamage(0.0f),
		DamageRange(0.0f),
		DamageRadius(0.0f),
		DamageSpeed(0.0f),
		MaxMana(0.0f),
		ManaRegenerationRate(0.0f),
		BaseDefense(0.0f),
		BaseHeal(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HpRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxBasicAttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DamageSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ManaRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseHeal;

	FGOCharacterStat operator+(const FGOCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FGOCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FGOCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

	FGOCharacterStat operator+(const FGOSkillStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FGOCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FGOSkillStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

};

//USTRUCT()
//struct FGOCharacterStatsBarInfo
//{
//	GENERATED_BODY()
//	
//	UPROPERTY()
//	float MaxHp;
//
//	UPROPERTY()
//	float CurrentHp;
//
//	UPROPERTY()
//	float MaxMana;
//
//	UPROPERTY()
//	float CurrentMana;
//
//	void FillFrom(FGOCharacterStat& InCharacterStat)
//	{
//		MaxHp = InCharacterStat.MaxHp;
//		MaxMana = InCharacterStat.MaxMana;
//	}
//};