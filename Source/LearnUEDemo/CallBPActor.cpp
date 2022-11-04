// Fill out your copyright notice in the Description page of Project Settings.


#include "CallBPActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACallBPActor::ACallBPActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("DemoCallBP"));   
}

void ACallBPActor::CallBPFuncName(FString name)
{
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("DemoTest"), ActorsToFind);
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsToFind);

	FSoftClassPath SoftClassPath(TEXT("/Game/BP/DemoTest.DemoTest_C"));
	UClass* bpClass = SoftClassPath.TryLoadClass<AActor>();
	UFunction*  bpFuc = nullptr;
	if(bpClass)
	{
		bpFuc = bpClass->FindFunctionByName(FName("PrintFunName"));
	}
	for (AActor* DemoTest : ActorsToFind)
	{
		if(DemoTest->Tags.Num() != 0 && DemoTest->Tags[0] == FName("DemoTest"))
		{
			UE_LOG(LogTemp, Warning, TEXT("find %d tags actor!"), DemoTest->Tags.Num());
			
			FString str = TEXT("wangleiTestFunc");
			if (bpFuc)
			{
				DemoTest->ProcessEvent(bpFuc, &str);
				// FFrame* frame = new FFrame(this, bpFuc, )
				// CallFunction
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Didn't find func"));
			}

			UE_LOG(LogTemp, Warning, TEXT("call func finish!"));
		}
	}
}


// Called when the game starts or when spawned
void ACallBPActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACallBPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

