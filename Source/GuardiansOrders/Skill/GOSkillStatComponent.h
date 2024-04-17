// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/GOSkillStat.h"
#include "GOSkillStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOSkillStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOSkillStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	FORCEINLINE void SetCurrentSkillType(const float InTypeNumber) { CurrentSkillType = InTypeNumber; }
	FORCEINLINE void SetBaseSkillStat(const FGOSkillStat& InBaseSkillStat) { BaseSkillStat = InBaseSkillStat; }
	FORCEINLINE FGOSkillStat GetTotalSkillStat() const { return BaseSkillStat; }

	void SetSkillStat(int8 InNewSkillType);
	void ResetSkillStat();

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentSkillType;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = SkillStat)
	FGOSkillStat BaseSkillStat;
};
