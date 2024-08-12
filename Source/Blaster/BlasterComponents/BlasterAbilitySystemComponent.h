// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BlasterAbilitySystemComponent.generated.h"

class ABlasterCharacter;
/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	friend ABlasterCharacter;
private:
	UPROPERTY()
	ABlasterCharacter* Character;
};
