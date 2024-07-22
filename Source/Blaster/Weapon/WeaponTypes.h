#pragma once

#define TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName="Assault Rifle"), // ²½Ç¹
	EWT_RocketLauncher UMETA(DisplayName="Rocket Launcher"), // »ð¼ýÍ²
	EWT_Pistol UMETA(DisplayName = "Pistol"), // ÊÖÇ¹
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"), // ³å·æÇ¹
	EWT_Shotgun UMETA(DisplayName = "Shotgun"), // ö±µ¯Ç¹
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"), // ¾Ñ»÷Ç¹
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"), // Áñµ¯·¢ÉäÆ÷ 

	EWT_MAX UMETA(DisplayName="DefaultMAX")

};