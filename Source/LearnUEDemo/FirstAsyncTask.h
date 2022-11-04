// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstAsyncTask.generated.h"

UCLASS()
class LEARNUEDEMO_API AFirstAsyncTask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirstAsyncTask(){};

	UFUNCTION(BlueprintCallable)
	void AsyncLoadTextFile(const FString& FilePath);

	UFUNCTION(BlueprintImplementableEvent)
	void OnFileLoaded(const FString& FileContent);

// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
//
// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

};
