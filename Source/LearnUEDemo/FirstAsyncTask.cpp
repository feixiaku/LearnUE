// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstAsyncTask.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_FileLoaded, FString);

class FTaskCompletion_LoadFileToString
{
	FTaskDelegate_FileLoaded TaskDelegate;
	FString FileContent;
	
public:
	FTaskCompletion_LoadFileToString(FString InFileContent, FTaskDelegate_FileLoaded InTaskDelegate)
		: TaskDelegate(InTaskDelegate), FileContent(InFileContent)
	{}
	
	// FTaskCompletion_LoadFileToString(FTaskDelegate_FileLoaded InTaskDelegate, FString InFileContent) :
	// 	TaskDelegate(InTaskDelegate), FileContent(MoveTemp(InFileContent))
	// {}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskCompletion_LoadFileToString, STATGROUP_TaskGraphTasks);
	}
	
	static ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}
	
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type currentThread, const FGraphEventRef MyCompletionGraphEvent)
	{
		check(IsInGameThread());
		TaskDelegate.ExecuteIfBound(MoveTemp(FileContent));
	}
};

FAutoConsoleTaskPriority CPrio_LoadFileToString(
	TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
	TEXT("Task and thread priority for file loading."),
	ENamedThreads::HighThreadPriority,
	ENamedThreads::NormalTaskPriority,
	ENamedThreads::HighTaskPriority 
);


class FTask_LoadFileToString
{
	FTaskDelegate_FileLoaded TaskDelegate;
	FString FilePath;

	FString FileContent;
public:
	// FTask_LoadFileToString(FString InFilePath, FTaskDelegate_FileLoaded InTaskDelegate) :
	// 	TaskDelegate(InTaskDelegate),FilePath(InFilePath)
	// {}

	FTask_LoadFileToString(FString InFilePath, FTaskDelegate_FileLoaded InTaskDelegate) :
		TaskDelegate(InTaskDelegate), FilePath(MoveTemp(InFilePath))
	{}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_LoadFileToString, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		return CPrio_LoadFileToString.Get();
	}

	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// load file from Content folder
		FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath);
		UE_LOG(LogTemp, Warning, TEXT("engine: %s"), *FPaths::EngineDir());
		UE_LOG(LogTemp, Warning, TEXT("automation: %s"), *FPaths::AutomationDir());
		UE_LOG(LogTemp, Warning, TEXT("launch: %s"), *FPaths::LaunchDir());
		UE_LOG(LogTemp, Warning, TEXT("project: %s"), *FPaths::ProjectDir());
		UE_LOG(LogTemp, Warning, TEXT("root: %s"), *FPaths::RootDir());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FPaths::ProjectContentDir());
		if(FPaths::FileExists(FullPath))
		{
			FFileHelper::LoadFileToString(FileContent, *FullPath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("==== file: %s not exist "), *FullPath);
			// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("file not exist!"));
		}

		// create completion task
		FGraphEventRef ChildTask = TGraphTask<FTaskCompletion_LoadFileToString>::CreateTask(nullptr
			, CurrentThread).ConstructAndDispatchWhenReady(FileContent, TaskDelegate);
		// MyCompletionGraphEvent->SetGatherThreadForDontCompleteUntil(ENamedThreads::GameThread);
		MyCompletionGraphEvent->DontCompleteUntil(ChildTask);
	}
};


void AFirstAsyncTask::AsyncLoadTextFile(const FString& FilePath)
{
	FTaskDelegate_FileLoaded TaskDelegate;
	TaskDelegate.BindUFunction(this, "OnFileLoaded");

	TGraphTask<FTask_LoadFileToString>::CreateTask(nullptr
		, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(FilePath, TaskDelegate);
}


