// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BlasterAttributeSet.generated.h"

class ABlasterCharacter;

// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBlasterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const; // to adjust damage that less equal MaxHealth
	
	
	/**
	* Player health
	*/
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, Health);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& LastHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& LastMaxHealth);

	/**
	* Player shield
	*/

	UPROPERTY(ReplicatedUsing = OnRep_MaxShield, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, MaxShield);

	UPROPERTY(ReplicatedUsing = OnRep_Shield, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, Shield);

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& LastShield);

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& LastMaxShield);

	UPROPERTY()
	ABlasterCharacter* OwningCharacter;
};
