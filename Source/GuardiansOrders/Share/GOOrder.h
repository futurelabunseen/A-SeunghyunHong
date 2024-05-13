// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOOrder.generated.h"

UENUM(BlueprintType)
enum class FGOOrderType : uint8
{
	None = 0,
	Move,
	Flash,
	BaseSkill, // Skill LMB
	Skill1, // Skill Q
	Skill2, // Skill W
	Skill3, // Skill E
	UltimateSkill // Skill R
};


USTRUCT(BlueprintType)
struct FGOOrder
{
	GENERATED_BODY()

	FGOOrder() :
		Type(FGOOrderType::None), dir(FVector_NetQuantizeNormal::Zero())
	{
	}

	FGOOrder(FGOOrderType InType) :
		Type(InType), dir(FVector_NetQuantizeNormal::Zero())
	{
	}

	FGOOrder(FGOOrderType InType, FVector_NetQuantizeNormal InDir) :
		Type(InType), dir(InDir)
	{
	}

	FGOOrder(FGOOrderType InType, FVector2D InDir) :
		Type(InType), dir(FVector_NetQuantizeNormal(InDir.Y, InDir.X, 0))
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGOOrderType Type;

	UPROPERTY(BlueprintReadWrite)
	FVector_NetQuantizeNormal dir;


	void Init()
	{
		dir = FVector_NetQuantizeNormal::Zero();
		Type = FGOOrderType::None;
	}

	bool IsMovementOrder() const
	{
		return
			Type == FGOOrderType::Move ||
			Type == FGOOrderType::Flash;
	}

	bool IsCastOrder() const
	{
		return
			Type == FGOOrderType::BaseSkill ||
			Type == FGOOrderType::Skill1 ||
			Type == FGOOrderType::Skill2 ||
			Type == FGOOrderType::Skill3 ||
			Type == FGOOrderType::UltimateSkill;
	}
};
