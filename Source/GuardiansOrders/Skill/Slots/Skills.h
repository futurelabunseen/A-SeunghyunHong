// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/Slots/SkillSlot.h"
#include "Skill/GOSkillBase.h"
#include "Skills.generated.h"

UCLASS()
class GUARDIANSORDERS_API ASkills : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkills();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// 스킬 초기화 함수
	void InitializeSkills(TArray<TSubclassOf<UGOSkillBase>> SkillClasses, TArray<FName> SkillStatDataRowName);

	FORCEINLINE const TArray<TObjectPtr<ASkillSlot>> &GetSkills() const { return Skills; }

	TObjectPtr<ASkillSlot> GetSkillAt(int32 Index) const {
		if (Skills.IsValidIndex(Index)) {
			return Skills[Index];
		}
		else 
		{
			return nullptr;
		}
	}

protected:
	UPROPERTY()
	TArray<TObjectPtr<ASkillSlot>> Skills;
};
