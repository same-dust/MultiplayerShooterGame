// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BlasterAttributeSet.generated.h"

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

	/**
	* Player health
	*/
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, Health);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, VisibleAnywhere, Category = "Blaster Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBlasterAttributeSet, MaxHealth);

	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,const FGameplayAttributeData& MaxAttribute,float NewMaxValue,const FGameplayAttribute& AffectedAttributeProperty) const; // to adjust damage that less equal MaxHealth

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& LastHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& LastMaxHealth);
};
