#pragma once

////////////////////////////////////////////////////////////////
// ECharacterSkills 
// 스킬의 활성화 방식입니다. (입력 구분)
////////////////////////////////////////////////////////////////

/**
	* BaseSkill:				기본 공격				마우스 클릭
	* Skill01:					스킬 1					Q키
	* Skill02:					스킬 2					W키
	* Skill03:					스킬 3					E키
	* UltimateSkill:			궁극기 스킬				R키
	* PotionSkill:				포션 스킬				F키
	* BuffSkill:				(TODO) 버프 스킬			숫자키
*/
UENUM(BlueprintType)
enum class ECharacterSkills : uint8
{
	None UMETA(Hidden),

	BaseSkill UMETA(DisplayName = "BaseSkill"),
	Skill01 UMETA(DisplayName = "Skill01"),
	Skill02 UMETA(DisplayName = "Skill02"),
	Skill03 UMETA(DisplayName = "Skill03"),
	UltimateSkill UMETA(DisplayName = "UltimateSkill"),
	PotionSkill UMETA(DisplayName = "PotionSkill"),
	BuffSkill UMETA(DisplayName = "BuffSkill"),

	Max UMETA(Hidden)
};



///////////////////////////////////////////////////////////////////
// ESkillTriggerType
// 스킬이 활성화되는 방식을 나타내며, 스킬의 타겟팅 메커니즘을 고려했습니다.
///////////////////////////////////////////////////////////////////

/**
	* Target: 사용자가 선택한 특정 대상을 향해 스킬이 활성화됩니다.
	* AutoTarget: 스킬이 자동으로 가장 가까운 적을 타겟팅하여 활성화됩니다.
		->	Target은 사용자가 직접적으로 대상을 선택해야 하지만,
		->	AutoTarget은 그러한 필요 없이 자동으로 타겟이 결정됩니다.
	* NonTargetDirection: 사용자가 결정한 방향으로 스킬이 활성화되지만 특정 대상이 필요하지 않습니다.
	* NonTargetRange: 방향이나 대상 선택을 요구하지 않고 사용자 주변 특정 범위에 영향을 미치는 스킬입니다.
	*
*/
UENUM(BlueprintType)
enum class ESkillTriggerType : uint8
{
	None UMETA(Hidden),

	Target UMETA(DisplayName = "Target"),
	AutoTarget UMETA(DisplayName = "AutoTarget"),
	NonTargetDirection UMETA(DisplayName = "NonTargetDirection"),
	NonTargetRange UMETA(DisplayName = "NonTargetRange"),

	Max UMETA(Hidden)
};



////////////////////////////////////////////////////////////////////////
// ESkillAffectType
// 스킬이 가질 수 있는 효과의 유형으로, 스킬 사용의 주요 결과나 영향을 나타냅니다.
////////////////////////////////////////////////////////////////////////

/**
	* Damage: 스킬이 적에게 피해를 입힙니다.
	* Block: 스킬이 방어 기능을 제공하여 오는 공격의 피해를 막거나 줄입니다.
	* Heal:	스킬이 플레이어나 팀원의 건강을 회복합니다.
	* Buff: 스킬이 플레이어나 팀원에게 일시적인 강화나 유익한 효과를 제공합니다.
	* Debuff: 스킬이 적에게 일시적인 약화 효과를 부여합니다.
*/
UENUM(BlueprintType)
enum class ESkillAffectType : uint8
{
	None UMETA(Hidden),

	Damage UMETA(DisplayName = "Damage"),
	Block UMETA(DisplayName = "Block"),
	Heal UMETA(DisplayName = "Heal"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff"),

	Max UMETA(Hidden)
};



////////////////////////////////////////////////////////////////////////
// EAutoDetectionType
////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EAutoDetectionType : uint8
{
	None = 0,
	Radius = 1,
	RadiusDegree = 2,
};