// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NameTypes.h"

namespace GOTagNames
{
	const static FName RED_TEAM_PLAYER_START = FName("RedTeam");
	const static FName BLUE_TEAM_PLAYER_START = FName("BlueTeam");
}

namespace GOCollisionChannel
{
	constexpr static ECollisionChannel ATTACK_CHANNEL = ECollisionChannel::ECC_GameTraceChannel1;
	constexpr static ECollisionChannel PROJECTILE_CHANNEL = ECollisionChannel::ECC_GameTraceChannel2;
}

namespace GOConsts
{
	constexpr uint8 PLAYER_MAX_NUM_SYSTEM = 4;
	constexpr float NO_HP = 0.f;
	constexpr float STUN_EVENT_DELAY_TIME = 1.f;
	constexpr float SELF_MOVE_AMOUNT = 1.f;
	constexpr float DEAD_EVENT_DELAY_TIME = 5.f;
}

namespace GOProjectile
{
	constexpr float PROJECTILE_INITIAL_SPEED = 500.f;
	constexpr float PROJECTILE_MAX_SPEED = 2000.f;
	constexpr float PROJECTILE_GRAVITY_SCALE = 0.f;
	constexpr float PROJECTILE_BASE_DAMAGE = 10.f;
	constexpr float PROJECTILE_DESTROY_DELAY_TIME = 0.5f;
}

namespace GOHeroCharacter
{
	const static FName KATNISS = FName(TEXT("Katniss"));
	const static FName ROGERS = FName(TEXT("Rogers"));
	const static FName BRIDE = FName(TEXT("Bride"));
	const static FName BEAST = FName(TEXT("Beast"));
}

namespace GOMaterial
{
	const static FName PERCENT = FName(TEXT("Percent"));
}

namespace GOSeamlessTravel
{
	constexpr TCHAR BATTLE_MAP_PATH[] = TEXT("/Game/Map/MyBattle");
}