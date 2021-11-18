// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "FreeDSendActor.generated.h"

UCLASS()
class AFreeDSendActor : public AActor
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION( BlueprintCallable, Category="FreeDSendActor|Functions" )
    void StartSend( class AVFreeDReceiveActor* InFreeDReceiver );
    UFUNCTION( BlueprintCallable, Category="FreeDSendActor|Functions" )
    void StopSend();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY( EditAnywhere, Category="FreeDSendActor|Properties" )
    float Interval;
    UPROPERTY()
    class AVFreeDReceiveActor* FreeDReceiver;

    int32 DataCounter;
    TArray<FString> FreeDDatas;
    FTimerHandle TimerHandle;
};
