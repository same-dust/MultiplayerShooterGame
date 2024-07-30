// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true; // that will change a lot to the server.we should fix bugs.
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// GameMode is ServerOnly,so LevelStartingTime Exist only in the server.
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}


void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer)
		{
			BlasterPlayer->OnMatchStateSet(MatchState);
		}
	}
}



void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = WarmupTime + MatchTime + CooldownTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
	
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState&&BlasterGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		PickLocationToRespawn(ElimmedController); // �������������Զ�ĵ�playerstart����,���û��������Ҵ����������
	}
}

void ABlasterGameMode::PickLocationToRespawn(AController* ElimmedController)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	TArray<AActor*> ExistPlayers;
	UGameplayStatics::GetAllActorsOfClass(this, ABlasterCharacter::StaticClass(), ExistPlayers);
	float MaxDistanceOfPlayers = 0;
	AActor* StartSpot=nullptr;
	if (ExistPlayers.Num() > 0)
	{
		for (auto Start : PlayerStarts)
		{
			FVector2D StartLocation(Start->GetActorLocation().X, Start->GetActorLocation().Y);
			float DistanceOfPlayers = 0;
			for (auto Player : ExistPlayers)
			{
				FVector2D PlayerLocation(Player->GetActorLocation().X, Player->GetActorLocation().Y);
				DistanceOfPlayers = DistanceOfPlayers +
					FMath::Abs(StartLocation.X - PlayerLocation.X) +
					FMath::Abs(StartLocation.Y - PlayerLocation.Y); //  ���о���
			}
			if (DistanceOfPlayers > MaxDistanceOfPlayers)
			{
				MaxDistanceOfPlayers = DistanceOfPlayers;
				StartSpot = Start;
			}
		}
	}
	else
	{
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		StartSpot = PlayerStarts[Selection];
	}

	if (ElimmedController && StartSpot)
	{
		RestartPlayerAtPlayerStart(ElimmedController, StartSpot);
	}
	else
	{
		// ����
	}
}


