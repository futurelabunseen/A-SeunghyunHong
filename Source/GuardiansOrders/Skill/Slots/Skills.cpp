// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Slots/Skills.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASkills::ASkills()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ASkills::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASkills::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkills::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 
}

void ASkills::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASkills, Skills);
}

void ASkills::InitializeSkills(TArray<TSubclassOf<UGOSkillBase>> SkillClasses, TArray<FName> SkillStatDataRowName)
{
	for (int32 i = 0; i < SkillClasses.Num(); ++i)
	{
		ASkillSlot* NewSkillSlot = GetWorld()->SpawnActor<ASkillSlot>();
		NewSkillSlot->InitializeSkillSlot(SkillClasses[i], SkillStatDataRowName[i]);
		//NewSkillSlot->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		NewSkillSlot->SetOwner(this->GetOwner()); // Set owner to the owner of ASkills (should be GOPlayerCharacter)
		Skills.Add(NewSkillSlot);

		UE_LOG(LogTemp, Warning, TEXT("Skills[%d]: %s"), i, *Skills[i].GetName());
	}
}