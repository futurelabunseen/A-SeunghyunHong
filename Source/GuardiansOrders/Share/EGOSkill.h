#pragma once

////////////////////////////////////////////////////////////////
// 스킬의 효과 타입입니다.
////////////////////////////////////////////////////////////////

/** 
	* InstantMelee:		즉시 시전, 무기 타격
	* EffectScope:			범위 지정, 사용자 중심 일정 반격
	* Projectile:				발사 공격
	* Block:			방어 기술
	* Healing:					치유 기술
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
	Block UMETA(DisplayName = "Block"),
	Healing UMETA(DisplayName = "Healing"), 
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

UENUM(BlueprintType)
enum class ESkillTriggerType : uint8
{
	None UMETA(Hidden),

	Target UMETA(DisplayName = "Target"),
	NonTargetDirection UMETA(DisplayName = "NonTargetDirection"),
	NonTargetRange UMETA(DisplayName = "NonTargetRange"),

	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESkillAffectType : uint8
{
	None UMETA(Hidden),

	Damage UMETA(DisplayName = "Damage"),
	Block UMETA(DisplayName = "Block"),
	Heal UMETA(DisplayName = "Heal"),
	Buff UMETA(DisplayName = "Buff"),

	Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////
// 스킬의 타입입니다. 데이터 테이블의 순서와 일치합니다. 
// GOSkillStatTable.csv 를 참고해주세요 
///////////////////////////////////////////////////////////////

/**
 * 캐릭터 타입입니다.
 * Lobby 에서 선택한 캐릭터로 Battle 에서 플레이할 수 있습니다.
 * Rogers : 탱커/근거리/파이터
 * Katniss : 원거리 딜러/원거리/캐리
 * Beast : 브루저/근거리/파이터
 * Bride : 힐러/원거리/서포터
 */
UENUM(BlueprintType)
enum class EGOHeroSkillType : uint8
{
	None UMETA(Hidden),

	GOSkill_Rogers_BaseSkill UMETA(DisplayName = "GOSkill_Rogers_BaseSkill"),
	GOSkill_Rogers_Skill01 UMETA(DisplayName = "GOSkill_Rogers_Skill01"),
	GOSkill_Rogers_Skill02 UMETA(DisplayName = "GOSkill_Rogers_Skill02"),
	GOSkill_Rogers_Skill03 UMETA(DisplayName = "GOSkill_Rogers_Skill03"),
	GOSkill_Rogers_UltimateSkill UMETA(DisplayName = "GOSkill_Rogers_UltimateSkill"),

	GOSkill_Katniss_BaseSkill UMETA(DisplayName = "GOSkill_Katniss_BaseSkill"),
	GOSkill_Katniss_Skill01 UMETA(DisplayName = "GOSkill_Katniss_Skill01"),
	GOSkill_Katniss_Skill02 UMETA(DisplayName = "GOSkill_Katniss_Skill02"),
	GOSkill_Katniss_Skill03 UMETA(DisplayName = "GOSkill_Katniss_Skill03"),
	GOSkill_Katniss_UltimateSkill UMETA(DisplayName = "GOSkill_Katniss_UltimateSkill"),

	GOSkill_Beast_BaseSkill UMETA(DisplayName = "GOSkill_Beast_BaseSkill"),
	GOSkill_Beast_Skill01 UMETA(DisplayName = "GOSkill_Beast_Skill01"),
	GOSkill_Beast_Skill02 UMETA(DisplayName = "GOSkill_Beast_Skill02"),
	GOSkill_Beast_Skill03 UMETA(DisplayName = "GOSkill_Beast_Skill03"),
	GOSkill_Beast_UltimateSkill UMETA(DisplayName = "GOSkill_Beast_UltimateSkill"),

	GOSkill_Bride_BaseSkill UMETA(DisplayName = "GOSkill_Bride_BaseSkill"),
	GOSkill_Bride_Skill01 UMETA(DisplayName = "GOSkill_Bride_Skill01"),
	GOSkill_Bride_Skill02 UMETA(DisplayName = "GOSkill_Bride_Skill02"),
	GOSkill_Bride_Skill03 UMETA(DisplayName = "GOSkill_Bride_Skill03"),
	GOSkill_Bride_UltimateSkill UMETA(DisplayName = "GOSkill_Bride_UltimateSkill")
};