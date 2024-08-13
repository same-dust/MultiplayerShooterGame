// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"

UBlasterAttributeSet::UBlasterAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitShield(0.f);
	InitMaxShield(200.f);
}

void UBlasterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBlasterAttributeSet, Health);
	DOREPLIFETIME(UBlasterAttributeSet, MaxHealth);
	DOREPLIFETIME(UBlasterAttributeSet, Shield);
	DOREPLIFETIME(UBlasterAttributeSet, MaxShield);
}

void UBlasterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		AdjustAttributeForMaxChange(Shield, MaxShield, NewValue, GetShieldAttribute());
	}
}

void UBlasterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	float DeltaValue{ 0.f };

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	ABlasterCharacter* TargetCharacter{ nullptr };

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor{ nullptr };
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ABlasterCharacter>(TargetActor);
	}
	OwningCharacter = TargetCharacter;
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		if (TargetCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Use PostGameplayEffectExecute"));
			// HandleHealthChanged
			
			// TargetCharacter->SetHealth(DeltaValue, SourceTags); need to refactor the function
			
			//TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags); 教程的函数
		}
	}
}

void UBlasterAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UBlasterAttributeSet::OnRep_Health(const FGameplayAttributeData& LastHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, Health, LastHealth);

	float CurrentHealth = Health.GetCurrentValue();
	float PreviousHealth = LastHealth.GetCurrentValue();

	OwningCharacter = IsValid(OwningCharacter) ? OwningCharacter : Cast<ABlasterCharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		OwningCharacter->UpdateHUDHealth();
	}

	if (CurrentHealth < PreviousHealth)
	{
		OwningCharacter->PlayHitReactMontage();
	}
}

void UBlasterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& LastMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, MaxHealth, LastMaxHealth);
}

void UBlasterAttributeSet::OnRep_Shield(const FGameplayAttributeData& LastShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, Shield, LastShield);

	float CurrentShield = Shield.GetCurrentValue();
	float PreviousShield = LastShield.GetCurrentValue();

	OwningCharacter = IsValid(OwningCharacter) ? OwningCharacter : Cast<ABlasterCharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		OwningCharacter->UpdateHUDShield();
	}

	if (CurrentShield < PreviousShield)
	{
		OwningCharacter->PlayHitReactMontage();
	}
}

void UBlasterAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& LastMaxShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, MaxShield, LastMaxShield);
}

