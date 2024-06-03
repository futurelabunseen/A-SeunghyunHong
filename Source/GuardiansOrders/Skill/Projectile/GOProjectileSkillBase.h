// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/GOSkillBase.h"
#include "Projectile/GOProjectile.h"
#include "GOProjectileSkillBase.generated.h"

class AGOProjectile;

/**
* projectile은 replicated actor 여야 한다.
* 서버가 생성하고 이동시키고 처리한다.
* 클라이언트는 복제된 버전을 봐야 한다.
*
* 서버에 있는 방법?
*
* UGOProjectileSkillBase는 ActivateSkill에서 발사체 스폰을 요청하는 역할만
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
	  * 스킬이 시작될 때 호출되는 함수로, 스킬 사용의 초기화 단계를 처리합니다.
	  */
	virtual void StartCast();

	/**
	  * 스킬이 활성화되는 동안 지속적으로 호출되며, 이는 주로 스킬의 진행 상태를 갱신하거나, 시간에 따라 변화하는 효과를 관리하는 데 사용됩니다.
	  */
	virtual void UpdateCast(float DeltaTime);

	/**
	  * 구체적인 스킬 내용으로, UpdateCast 내에서 호출되지 않을까- 라고 생각 중
	  * 스킬에 따라서, 즉발이거나 시간 차를 두고 공격이 될 수 있으므로?
	  */
	virtual void ActivateSkill();

	/**
	  * 스킬 사용이 완료됐을 때 호출되는 함수로, 스킬의 결과를 처리하고 마무리하는 작업을 수행합니다.
	  */
	virtual void FinishCast();

	/**
	  * 스킬이 중단될 때 호출되는 함수로, 예를 들어 캐릭터가 피격당했을 때 스킬 사용을 중단시키고 필요한 정리 작업을 수행합니다.
	  */
	virtual void InterruptedCast();

	/**
	  * 스킬의 VFX 효과를 재생합니다.
	  */
	virtual void ActivateEffect();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGOProjectile> ProjectileClass;
};

