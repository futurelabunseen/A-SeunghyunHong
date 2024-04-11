#pragma once

#include "CoreMinimal.h"

////////////////////////////////////////////////////////////////
// 캐릭터 타입입니다. 데이터 테이블의 순서와 일치합니다. 
// GOCharacterStatTable.csv 를 참고해주세요 
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
enum class EHeroType : uint8
{
    None UMETA(Hidden),

    Rogers UMETA(DisplayName = "Rogers"),
    Katniss UMETA(DisplayName = "Katniss"),
    Beast UMETA(DisplayName = "Beast"),
    Bride UMETA(DisplayName = "Bride"),
};


/////////////////////////////////////////////////////////////////
// 캐릭터 관련 정보 : 데이터 애셋에서 지정합니다. 
////////////////////////////////////////////////////////////////

/**
 * 직업군입니다.
 * 탱커, 원거리 딜러, 브루저, 힐러가 있습니다.
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
 * 공격 범위입니다.
 * 근거리와 원거리가 있습니다.
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
 * 전형입니다.
 * 파이터, 캐리, 서포터가 있습니다.
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
// 팀전 배틀 게임에 필요한 데이터입니다.
////////////////////////////////////////////////////////////////

/**
 * 팀 정보입니다.
 * RedTeam과 BlueTeam이 있습니다.
 */
UENUM(BlueprintType)
enum class ETeamType : uint8
{
    None UMETA(Hidden),

    RedTeam UMETA(DisplayName = "Red Team"),
    BlueTeam UMETA(DisplayName = "Blue Team"),

    Max UMETA(Hidden)
};