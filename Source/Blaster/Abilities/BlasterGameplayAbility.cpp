// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameplayAbility.h"

UBlasterGameplayAbility::UBlasterGameplayAbility()
{
}

void UBlasterGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		ABlasterCharacter* Character = Cast<ABlasterCharacter>(ActorInfo->AvatarActor);
		if (Character)
		{
			//Character->PlayDanceMontage();
			UE_LOG(LogTemp, Warning, TEXT("Dancing Ability Active by Input"));
		}
		

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

}