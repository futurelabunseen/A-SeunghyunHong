// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/GOSkillBase.h"
#include "SkillSlot.generated.h"

UCLASS()
class GUARDIANSORDERS_API ASkillSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillSlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE TObjectPtr<UGOSkillBase> GetSkillInstance() const {
		return SkillInstance;
	}

	// GOCharacterBase에 있던 애들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills", Replicated)
	TSubclassOf<UGOSkillBase> SkillClassType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills", Replicated)
	 TObjectPtr<UGOSkillBase> SkillInstance;	

	 // 스킬 클래스로 이 슬롯을 초기화
	 void InitializeSkillSlot(TSubclassOf<UGOSkillBase> NewSkillClass, FName SkillStatDataTableRowName);
};
