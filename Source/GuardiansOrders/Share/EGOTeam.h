#pragma once


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

    ET_RedTeam UMETA(DisplayName = "Red Team"),
    ET_BlueTeam UMETA(DisplayName = "Blue Team"),
    ET_NoTeam UMETA(DisplayName = "NoTeam"),

    ET_Max UMETA(DisplayName = "DefaultMax")
};