// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define ECC_HitBox ECollisionChannel::ECC_GameTraceChannel2

// must deep understand the collision of unreal engine. it's useful and important.

// UE5��һЩ����

/*
	delegates(ί��)--�������ಥ����̬ί��
	�������:���Ʊ��������ڱ�������ʱ���ݱ�������ִ��ReplicatedUsing��Ӧ��OnRep_������
			���ƺ�����Client RPC��Server RPC��Multicast RPC
	Cast<>(),Super::function();


*/


/*
TSubclassOf<ACasing> CasingClass; ?ʲô�����ࣿ
*/