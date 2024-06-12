// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOMagicCircle.generated.h"

UCLASS()
class GUARDIANSORDERS_API AGOMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AGOMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;


};
