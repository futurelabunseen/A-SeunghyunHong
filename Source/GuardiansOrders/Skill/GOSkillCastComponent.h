// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOSkillCastComponent.generated.h"


class GOSkillCastComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUARDIANSORDERS_API UGOSkillCastComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGOSkillCastComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnUpdateCast(float DeltaTime) {}

private:
	bool bIsCastable = false;
	bool bIsCasting = false;


};

/*
EGOSkillType에 따라 다르게 처리
*/