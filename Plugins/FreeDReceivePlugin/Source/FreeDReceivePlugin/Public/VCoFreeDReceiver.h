// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "VCoroutineBase.h"

class FREEDRECEIVEPLUGIN_API FVCoFreeDReceiver : public FVCoroutineBase
{
public:
    FVCoFreeDReceiver( float InDelay, float InInterval, TFunction<void( float )>&& InAction, TFunction<void( float )>&& InDelayAction, 
        TFunction<float( void )>&& InWorldTimeSeconds );

protected:
    void Run( float InDeltaTime ) override;

private:
    float Delay;
    float Interval;
    float PrevTime;
    TFunction<void( float )> Action;
    TFunction<void( float )> DelayAction;
    TFunction<float( void )> WorldTimeSeconds;
};
