// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define ECC_HitBox ECollisionChannel::ECC_GameTraceChannel2

// must deep understand the collision of unreal engine. it's useful and important.

// UE5的一些操作

/*
	delegates(委托)--单播，多播，动态委托
	网络操作:复制变量，会在变量更新时传递变量（并执行ReplicatedUsing相应的OnRep_函数）
			复制函数，Client RPC，Server RPC，Multicast RPC
	Cast<>(),Super::function();


*/


/*
TSubclassOf<ACasing> CasingClass; ?什么是子类？
*/