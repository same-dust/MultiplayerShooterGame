#pragma once

UENUM(BlueprintType)
enum class EBlasterAbilityInput :uint8
{
	EBAI_None UMETA(DisplayName="None"),
	EBAI_Confirm UMETA(DisplayName="Confirm"),
	EBAI_Cancel UMETA(DisplayName="Cancel"),
	EBAI_Fire UMETA(DisplayName="Fire"),

	EBAI_MAX UMETA(DisplayName = "DefaultMAX")
};
