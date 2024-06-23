// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "Projectile/GOProjectile.h"
#include "GOProjectileSkillBase.generated.h"

class AGOProjectile;

/**
* projectile�� replicated actor ���� �Ѵ�.
* ������ �����ϰ� �̵���Ű�� ó���Ѵ�.
* Ŭ���̾�Ʈ�� ������ ������ ���� �Ѵ�.
*
* ������ �ִ� ���?
*
* UGOProjectileSkillBase�� ActivateSkill���� �߻�ü ������ ��û�ϴ� ���Ҹ�
*/

UCLASS()
class GUARDIANSORDERS_API UGOProjectileSkillBase : public UGOSkillBase
{
	GENERATED_BODY()
	
public:
	UGOProjectileSkillBase();
	virtual void PostInitProperties() override;

public:
	/**
	  * ��ų�� ���۵� �� ȣ��Ǵ� �Լ���, ��ų ����� �ʱ�ȭ �ܰ踦 ó���մϴ�.
	  */
	virtual void StartCast();

	virtual void Tick(float DeltaTime) override;

	/**
	  * ��ų�� Ȱ��ȭ�Ǵ� ���� ���������� ȣ��Ǹ�, �̴� �ַ� ��ų�� ���� ���¸� �����ϰų�, �ð��� ���� ��ȭ�ϴ� ȿ���� �����ϴ� �� ���˴ϴ�.
	  */
	virtual void UpdateCast(float DeltaTime);

	/**
	  * ��ü���� ��ų ��������, UpdateCast ������ ȣ����� ������- ��� ���� ��
	  * ��ų�� ����, ����̰ų� �ð� ���� �ΰ� ������ �� �� �����Ƿ�?
	  */
	virtual void ActivateSkill();

	/**
	  * ��ų ����� �Ϸ���� �� ȣ��Ǵ� �Լ���, ��ų�� ����� ó���ϰ� �������ϴ� �۾��� �����մϴ�.
	  */
	virtual void FinishCast();

	/**
	  * ��ų�� �ߴܵ� �� ȣ��Ǵ� �Լ���, ���� ��� ĳ���Ͱ� �ǰݴ����� �� ��ų ����� �ߴܽ�Ű�� �ʿ��� ���� �۾��� �����մϴ�.
	  */
	virtual void InterruptedCast();

	/**
	  * ��ų�� VFX ȿ���� ����մϴ�.
	  */
	virtual void ActivateEffect();

	void SetSpeed();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGOProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AGOProjectile> Projectile;

	// Spread angle for the projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpreadAngle;

	// Number of projectiles to spawn in a spread
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 NumProjectiles;

	// Flag to indicate if this skill uses spread
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bIsSpreadSkill;
};

