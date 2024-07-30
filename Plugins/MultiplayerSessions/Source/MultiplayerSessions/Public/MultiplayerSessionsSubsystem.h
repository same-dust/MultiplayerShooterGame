// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

//
// Declaring our own custom delegates for the Menu class to bind callbacks to
//
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();

	//
	// To handle session functionality. The Menu class will call these
	//
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	//
	// Our own custom delegates for the Menu class to bind callbacks to
	//
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:

	//
	// Internal callbacks for the delegates we'll add to the Online Session Interface delegate list.
	// These don't need to be called outside this class.
	// ����ͨ���鿴delegate�Ķ������˽���Щ�ص�������ʲô����
	//
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	//
	// To add to the Online Session Interface delegate list.
	// We'll bind our MultiplayerSessionSubsystem internal callbacks to these.
	//
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	bool bCreateSessionOnDestroy{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
};



/*
How to CreateSession�Ĵ�������
	UMenu::Initialize()�У�ΪHostButton��OnClicked�¼�bind�ص�����HostButtonClicked(),
	HostButtonClicked()�����CreateSession(),
	��CreateSession()�У����CreateSessionCompleteDelegate���ί����ӵ�SessionInterface��Ӧ��ί�ж��в����ؾ����������CreateSessionCompleteDelegateHandle��
	���SessionInterface�ɹ�����Session,��֪ͨ��Ӧί�ж��е�ί��(CreateSessionCompleteDelegate)ִ����ص�����OnCreateSessionComplete()
	��OnCreateSessionComplete()�л����MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful)��������MenuSetup()���������õ�ί��
	MultiplayerOnCreateSessionComplete��������󶨵�OnCreateSession()
	��OnCreateSession()�У�����ʵ�ֽ��ؿ�server travel��Lobby Map��
*/

/*
How to JoinSession�Ĵ�������
	Find Session
		UMenu::Initialize()��,ΪJoinButton��OnClicked�¼�bind�ص�����JoinButtonClicked(),
		JoinButtonClicked()�����FindSessions()
		��FindSessions()��,���FindSessionsCompleteDelegate���ί����ӵ�SessionInterface��Ӧ��ί�ж��в����ؾ����������FindSessionsCompleteDelegateHandle��
		����SessionInterfaceѰ��Session��Ȼ��֪ͨ��Ӧί�ж��е�ί��(FindSessionsCompleteDelegate)ִ����ص�����OnFindSessionsComplete()
		��OnFindSessionsComplete()�У�����ҵ���Session������0�������ί�й㲥�������ҵ���Sessionsһ������------MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
		MultiplayerOnFindSessionsComplete��������󶨵�OnFindSessions()���ú�����������ҽ�����ҵ���һ��ƥ��Ľ���󣬻����JoinSession(Result)
	Join Session
		��JoinSession()��,���JoinSessionCompleteDelegate���ί����ӵ�SessionInterface��Ӧ��ί�ж��в����ؾ����������JoinSessionCompleteDelegateHandle��
		����SessionInterface�᳢�Լ���Session��Ȼ��֪ͨ��Ӧί�ж��е�ί��(JoinSessionCompleteDelegate)ִ����ص�����OnJoinSessionComplete()
		��OnJoinSessionComplete()�У���ʹ��MultiplayerOnJoinSessionComplete.Broadcast(Result)����ί�н��й㲥
		MultiplayerOnJoinSessionComplete��������󶨵�OnJoinSession(),�ú�����ʹ��ClientTravel����Session����������Ϸ��
*/

/*
����session�Ƚϼ�
	��DestroySession()�У���DestroySessionCompleteDelegate��ӵ�SessionInterface��Ӧ��ί�ж��У��������صľ�����浽DestroySessionCompleteDelegateHandle������
	�������Ự���٣�ί�л����OnDestroySessionComplete����֪ͨ��������
*/