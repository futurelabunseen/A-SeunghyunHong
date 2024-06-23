// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GOPlayerCharacter.h"
#include "GORogersCharacter.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class GUARDIANSORDERS_API AGORogersCharacter : public AGOPlayerCharacter
{
	GENERATED_BODY()

public:
	AGORogersCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnBaseSkill() override;
	virtual void OnSkillQ() override;
	virtual void OnSkillW() override;
	virtual void OnSkillE() override;
	virtual void OnSkillR() override;

protected:
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

public:
	// 방패 상태를 관리하기 위한 함수 추가
	void ActivateShield();
	void DeactivateShield();
	bool IsShieldBlocking(const FVector& AttackDirection) const;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ShieldEffect;

	TObjectPtr<UNiagaraComponent> FXShield;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> ShieldSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield", meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

private:
	bool bShieldActive;
	FVector ShieldDirection;
	float MaxBlockAngle; // 방패가 공격을 막을 수 있는 각도

// ======== IPlaySkillAnimInterface ========
//	virtual UGOSkillCastComponent* GetSkillCastComponent()
//	{
//		return SkillCastComponent;
//	}
//
//	virtual void PlaySkillAnim(UGOSkillBase* CurrentSkill) override;
//
//

};