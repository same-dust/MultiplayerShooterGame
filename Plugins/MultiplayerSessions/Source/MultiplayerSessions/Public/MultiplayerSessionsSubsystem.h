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
	// 可以通过查看delegate的定义来了解这些回调函数有什么参数
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
How to CreateSession的大致流程
	UMenu::Initialize()中，为HostButton的OnClicked事件bind回调函数HostButtonClicked(),
	HostButtonClicked()会调用CreateSession(),
	在CreateSession()中，会把CreateSessionCompleteDelegate这个委托添加到SessionInterface对应的委托队列并返回句柄（保存在CreateSessionCompleteDelegateHandle）
	如果SessionInterface成功创建Session,会通知对应委托队列的委托(CreateSessionCompleteDelegate)执行其回调函数OnCreateSessionComplete()
	在OnCreateSessionComplete()中会调用MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful)，这是在MenuSetup()函数中设置的委托
	MultiplayerOnCreateSessionComplete调用与其绑定的OnCreateSession()
	在OnCreateSession()中，函数实现将关卡server travel到Lobby Map。
*/

/*
How to JoinSession的大致流程
	Find Session
		UMenu::Initialize()中,为JoinButton的OnClicked事件bind回调函数JoinButtonClicked(),
		JoinButtonClicked()会调用FindSessions()
		在FindSessions()中,会把FindSessionsCompleteDelegate这个委托添加到SessionInterface对应的委托队列并返回句柄（保存在FindSessionsCompleteDelegateHandle）
		接着SessionInterface寻找Session，然后通知对应委托队列的委托(FindSessionsCompleteDelegate)执行其回调函数OnFindSessionsComplete()
		在OnFindSessionsComplete()中，如果找到的Session数大于0，会调用委托广播，并将找到的Sessions一并传入------MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
		MultiplayerOnFindSessionsComplete调用与其绑定的OnFindSessions()，该函数会遍历查找结果，找到第一个匹配的结果后，会调用JoinSession(Result)
	Join Session
		在JoinSession()中,会把JoinSessionCompleteDelegate这个委托添加到SessionInterface对应的委托队列并返回句柄（保存在JoinSessionCompleteDelegateHandle）
		接着SessionInterface会尝试加入Session，然后通知对应委托队列的委托(JoinSessionCompleteDelegate)执行其回调函数OnJoinSessionComplete()
		在OnJoinSessionComplete()中，会使用MultiplayerOnJoinSessionComplete.Broadcast(Result)来对委托进行广播
		MultiplayerOnJoinSessionComplete调用与其绑定的OnJoinSession(),该函数会使用ClientTravel到该Session，即加入游戏。
*/

/*
销毁session比较简单
	在DestroySession()中，把DestroySessionCompleteDelegate添加到SessionInterface对应的委托队列，并将返回的句柄保存到DestroySessionCompleteDelegateHandle就行了
	这样当会话销毁，委托会调用OnDestroySessionComplete进行通知。。。。
*/