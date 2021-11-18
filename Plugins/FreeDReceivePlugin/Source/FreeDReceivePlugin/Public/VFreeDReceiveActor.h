// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "VFreeDReceiveActor.generated.h"

UCLASS() 
class FREEDRECEIVEPLUGIN_API AVFreeDReceiveActor : public AActor
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION( BlueprintCallable, Category="FreeDReceiveActor|Function" )
    void ReceiveMessage( const FString& InMessage );
    UFUNCTION( BlueprintCallable, Category="FreeDReceiveActor|Function" )
    void StartReceive();
    UFUNCTION( BlueprintCallable, Category="FreeDReceiveActor|Function" )
    void StopReceive();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay( const EEndPlayReason::Type InEndPlayReason ) override;

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FVFreeDReceiveEventDelegate, FRotator, InCamRotation, FVector, InCamLocation, FVector2D, InCamZoomFocus );
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVFreeDReceiveMessageDelegate, FString, InMessage );
    UPROPERTY( BlueprintAssignable, Category="FreeDReceiveActor|Events" )
    FVFreeDReceiveEventDelegate OnFreeDReceiveEventDelegate;
    UPROPERTY( BlueprintAssignable, Category="FreeDReceiveActor|Events" )
    FVFreeDReceiveMessageDelegate OnFreeDReceiveMessageDelegate;

    UPROPERTY( EditAnywhere, Category="FreeDReceiveActor|Properties" )
    float Delay;
    UPROPERTY( EditAnywhere, Category="FreeDReceiveActor|Properties" )
    float Interval;
    UPROPERTY( EditAnywhere, Category="FreeDReceiveActor|Properties" )
    bool OutputLog;

private:
    FDelegateHandle TickerHandle;
    TSharedPtr<class FVCoroutineBase> Coroutine;
    TQueue<FString> ReceiveQueue;
    TQueue<FString> IntermediateQueue;
};
