// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameplayAbility.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UBlasterGameplayAbility::UBlasterGameplayAbility()
{

}

void UBlasterGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        Character = Cast<ABlasterCharacter>(ActorInfo->AvatarActor);
        if (Character && Character->GetMesh())
        {
            UAnimMontage* DanceMontage = Character->GetDanceMontage();
            if (DanceMontage)
            {
                // 创建并启动任务
                UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DanceMontage, 1.0f, NAME_None, false);
                Character->SetbIsDancing(true);
                // 绑定任务完成时的回调
                Task->OnCompleted.AddDynamic(this, &UBlasterGameplayAbility::OnMontageCompleted);
                Task->OnInterrupted.AddDynamic(this, &UBlasterGameplayAbility::OnMontageInterrupted);
                Task->OnCancelled.AddDynamic(this, &UBlasterGameplayAbility::OnMontageCancelled);

                // 启动任务
                Task->Activate();

                UE_LOG(LogTemp, Warning, TEXT("Dancing Ability Active by Input"));
            }
        }
    }
}

void UBlasterGameplayAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    Character->SetbIsDancing(false);
}

void UBlasterGameplayAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    Character->SetbIsDancing(false);
}

void UBlasterGameplayAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    Character->SetbIsDancing(false);
}
