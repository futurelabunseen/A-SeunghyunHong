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
UENUM()
enum class ECharacterSkills : uint8
{
	None = 0 UMETA(Hidden),

	BaseSkill UMETA(DisplayName = "BaseSkill"),
	Skill01 UMETA(DisplayName = "Skill01"),
	Skill02 UMETA(DisplayName = "Skill02"),
	Skill03 UMETA(DisplayName = "Skill03"),
	UltimateSkill UMETA(DisplayName = "UltimateSkill"),
	PotionSkill UMETA(DisplayName = "PotionSkill"),
	BuffSkill UMETA(DisplayName = "BuffSkill"),

	Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////
// ECharacterSpells 
// 스펠의 활성화 방식입니다. (입력 구분)
////////////////////////////////////////////////////////////////

/**
	* Spell01:					스펠 1: Ghost				D키
	* Spell02:					스펠 2: Heal					F키
*/

UENUM()
enum class ECharacterSpells : uint8
{
	None = 0 UMETA(Hidden),

	Spell01 UMETA(DisplayName = "Spell01"),
	Spell02 UMETA(DisplayName = "Spell02"),
	Spell03 UMETA(DisplayName = "Spell03"),

	Max UMETA(Hidden)
};


UENUM()
enum class ESpellType : uint8
{
	None = 0 UMETA(Hidden),

	GOSpell_Flash UMETA(DisplayName = "GOSpell_Flash"),
	GOSpell_Heal UMETA(DisplayName = "GOSpell_Heal"),
	GOSpell_Ghost UMETA(DisplayName = "GOSpell_Ghost"),

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
		->  AutoTargetRadius은 일정 범위 내 누군가 이싿면 타켓으로 결정하고
		->  AutoTargetRadiusDegree은 일정 범위의 일정 각도 내 누군가 이싿면 타켓으로 결정합니다.
	* NonTargetDirection: 사용자가 결정한 방향으로 스킬이 활성화되지만 특정 대상이 필요하지 않습니다.
	* NonTargetRange: 방향이나 대상 선택을 요구하지 않고 사용자 주변 특정 범위에 영향을 미치는 스킬입니다.
	*
*/
UENUM(BlueprintType)
enum class ESkillTriggerType : uint8
{
	None UMETA(Hidden),

	Target UMETA(DisplayName = "Target"),
	AutoTargetRadius UMETA(DisplayName = "AutoTargetRadius"),
	AutoTargetRadiusDegree UMETA(DisplayName = "AutoTargetRadiusDegree"),
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
	* Defense: 스킬이 방어 기능을 제공하여 오는 공격의 피해를 막거나 줄입니다.
	* Heal:	스킬이 플레이어나 팀원의 건강을 회복합니다.
	* Buff: 스킬이 플레이어나 팀원에게 일시적인 강화나 유익한 효과를 제공합니다.
	* Debuff: 스킬이 적에게 일시적인 약화 효과를 부여합니다.
*/
UENUM(BlueprintType)
enum class ESkillAffectType : uint8
{
	None UMETA(Hidden),

	Damage UMETA(DisplayName = "Damage"),
	Defense UMETA(DisplayName = "Defense"),
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


////////////////////////////////////////////////////////////////////////
// ESkillState
////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	None UMETA(DisplayName = "None"),
	ReadyToCast UMETA(DisplayName = "Ready to Cast"),
	Casting UMETA(DisplayName = "Casting"),
	CastCancelled UMETA(DisplayName = "Cast Cancelled"),
	OnCooldown UMETA(DisplayName = "On Cooldown"),
	NoMana UMETA(DisplayName = "No Mana"),
	Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////////////
// ESkillCollisionType
////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class ESkillCollisionType : uint8
{
	None UMETA(DisplayName = "None"),

	LineTraceSingle   UMETA(DisplayName = "Line Trace Single"),
	LineTraceMulti    UMETA(DisplayName = "Line Trace Multi"),
	SweepSingle       UMETA(DisplayName = "Sweep Single"),
	SweepMulti        UMETA(DisplayName = "Sweep Multi"),
	OverlapMulti      UMETA(DisplayName = "Overlap Multi"),

	Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////
// ESkillCastType 
// 스킬의 시전 방식을 나타내며, 각 스킬이 어떻게 발동되는지를 설명합니다.
////////////////////////////////////////////////////////////////

/**
	* Instant:		즉발형 스킬		스킬 버튼을 누르자마자 즉시 효과가 발생합니다.
	* Projectile:	발사형 스킬		스킬 사용 시 발사체가 발사되어 목표물에 도달합니다.
	* Delayed:		지연형 스킬		시전 후 일정 시간이 지나야 효과가 발생합니다.
	* AreaOfEffect:	범위형 스킬		특정 영역에 영향을 미치는 스킬입니다.
	* Charge:		차징형 스킬		시전 시간을 길게 가져갈수록 더 큰 효과를 발휘합니다.
*/
UENUM(BlueprintType)
enum class ESkillCastType : uint8
{
	Instant UMETA(DisplayName = "Instant"),
	Projectile UMETA(DisplayName = "Projectile"),
	Delayed UMETA(DisplayName = "Delayed"),
	AreaOfEffect UMETA(DisplayName = "Area of Effect"),
	Charge UMETA(DisplayName = "Charge"),

	Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////
// EParticleSpawnLocation 
// Particle이 생성되는 위치를 나타내며, 스킬 효과의 위치를 설명합니다.
////////////////////////////////////////////////////////////////

/**
	* Caster:			시전자 위치		Particle이 스킬 시전자의 위치에서 생성됩니다.
	* Target:			대상 위치		Particle이 스킬 대상의 위치에서 생성됩니다.
	* Projectile:		발사체 위치		Particle이 스킬 발사체의 위치에서 생성됩니다.
	* Ground:			지면 위치		Particle이 지면의 특정 위치에서 생성됩니다.
	* AroundCaster:		시전자 주변		Particle이 시전자 주변에서 생성됩니다.
	* AroundTarget:		대상 주변		Particle이 대상 주변에서 생성됩니다.
	* CustomLocation:	사용자 정의 위치	Particle이 특정 사용자 정의 위치에서 생성됩니다.
*/
UENUM(BlueprintType)
enum class EParticleSpawnLocation : uint8
{
	Caster UMETA(DisplayName = "Caster"),
	Target UMETA(DisplayName = "Target"),
	Projectile UMETA(DisplayName = "Projectile"),
	Ground UMETA(DisplayName = "Ground"),
	AroundCaster UMETA(DisplayName = "Around Caster"),
	AroundTarget UMETA(DisplayName = "Around Target"),
	CustomLocation UMETA(DisplayName = "Custom Location"),

	Max UMETA(Hidden)
};
