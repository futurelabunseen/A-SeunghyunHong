// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleSpell/GOMagicCircle.h"
#include "Components/DecalComponent.h"

AGOMagicCircle::AGOMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircle");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AGOMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGOMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

