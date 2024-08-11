// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Blaster/BlasterTypes/BlasterAbilityInput.h"
#include "BlasterGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UBlasterGameplayAbility();

	UPROPERTY(EditAnywhere, Category = "Abilities")
	EBlasterAbilityInput AbilityInputID{ EBlasterAbilityInput::EBAI_None };

};
