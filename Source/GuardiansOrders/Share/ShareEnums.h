#pragma once

#include "CoreMinimal.h"

////////////////////////////////////////////////////////////////
// ĳ���� Ÿ���Դϴ�. ������ ���̺��� ������ ��ġ�մϴ�. 
// GOCharacterStatTable.csv �� �������ּ��� 
///////////////////////////////////////////////////////////////

/**
 * ĳ���� Ÿ���Դϴ�. 
 * Lobby ���� ������ ĳ���ͷ� Battle ���� �÷����� �� �ֽ��ϴ�.
 * Rogers : ��Ŀ/�ٰŸ�/������
 * Katniss : ���Ÿ� ����/���Ÿ�/ĳ��
 * Beast : �����/�ٰŸ�/������
 * Bride : ����/���Ÿ�/������
 */
UENUM(BlueprintType)
enum class EHeroType : uint8
{
    None UMETA(Hidden),

    Rogers UMETA(DisplayName = "Rogers"),
    Katniss UMETA(DisplayName = "Katniss"),
    Beast UMETA(DisplayName = "Beast"),
    Bride UMETA(DisplayName = "Bride"),
};


/////////////////////////////////////////////////////////////////
// ĳ���� ���� ���� : ������ �ּ¿��� �����մϴ�. 
////////////////////////////////////////////////////////////////

/**
 * �������Դϴ�.
 * ��Ŀ, ���Ÿ� ����, �����, ������ �ֽ��ϴ�.
 */
UENUM(BlueprintType)
enum class ERoleType : uint8
{
    None UMETA(Hidden),

    Tanker UMETA(DisplayName = "Tanker"),
    Dealer UMETA(DisplayName = "Dealer"),
    Bruiser UMETA(DisplayName = "Bruiser"),
    Healer UMETA(DisplayName = "Healer"),

    Max UMETA(Hidden)
};


/**
 * ���� �����Դϴ�.
 * �ٰŸ��� ���Ÿ��� �ֽ��ϴ�.
 */
UENUM(BlueprintType)
enum class EAttackRange : uint8
{
    None UMETA(Hidden),

    Melee UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged"),

    Max UMETA(Hidden)
};


/**
 * �����Դϴ�.
 * ������, ĳ��, �����Ͱ� �ֽ��ϴ�.
 */
UENUM(BlueprintType)
enum class EArchetype : uint8
{
    None UMETA(Hidden),

    Fighter UMETA(DisplayName = "Fighter"),
    Carry UMETA(DisplayName = "Carry"),
    Supporter UMETA(DisplayName = "Supporter"),

    Max UMETA(Hidden)
};

////////////////////////////////////////////////////////////////
// ���� ��Ʋ ���ӿ� �ʿ��� �������Դϴ�.
////////////////////////////////////////////////////////////////

/**
 * �� �����Դϴ�.
 * RedTeam�� BlueTeam�� �ֽ��ϴ�.
 */
UENUM(BlueprintType)
enum class ETeamType : uint8
{
    None UMETA(Hidden),

    RedTeam UMETA(DisplayName = "Red Team"),
    BlueTeam UMETA(DisplayName = "Blue Team"),

    Max UMETA(Hidden)
};