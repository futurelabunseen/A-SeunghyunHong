#pragma once

////////////////////////////////////////////////////////////////
// 스킬의 효과 타입입니다.
////////////////////////////////////////////////////////////////

/** 
	* InstantMelee:		즉시 시전, 무기 타격
	* EffectScope:			범위 지정, 사용자 중심 일정 반격
	* Projectile:				발사 공격
	* Targeting:				(TODO) 타겟팅
	* Potion:					즉시 체력 회복 20%
	* Buff:						(TODO) 버프
*/
UENUM(BlueprintType)
enum class EGOSkillType : uint8
{
	None UMETA(Hidden),

	InstantMelee,
	EffectScope UMETA(DisplayName = "EffectScope"),
	Projectile UMETA(DisplayName = "Projectile"),
	Targeting UMETA(DisplayName = "Targeting"),
	Potion UMETA(DisplayName = "Potion"),
	Buff UMETA(DisplayName = "Buff"),

	Max UMETA(Hidden)
};


////////////////////////////////////////////////////////////////
// 스킬의 활성화 방식입니다.
////////////////////////////////////////////////////////////////

/**
	* BaseSkill:				기본 공격					마우스 클릭
	* Skill01:					스킬 1						Q키
	* Skill02:					스킬 2						W키
	* Skill03:					스킬 3						E키
	* UltimateSkill:		궁극기 스킬				R키 
	* PotionSkill:			포션 스킬					F키
	* BuffSkill:				(TODO) 버프 스킬		숫자키
*/

UENUM(BlueprintType)
enum class EGOSkillCategory : uint8
{
	None UMETA(Hidden),

	BaseSkill UMETA(DisplayName = "BaseSkill"),
	Skill01 UMETA(DisplayName = "Skill01"),
	Skill02 UMETA(DisplayName = "Skill02"),
	Skill03 UMETA(DisplayName = "Skill03"),
	UltimateSkill UMETA(DisplayName = "UltimateSkill"),
	PotionSkill UMETA(DisplayName= "PotionSkill"),
	BuffSkill UMETA(DisplayName = "BuffSkill"),

	Max UMETA(Hidden)
};
