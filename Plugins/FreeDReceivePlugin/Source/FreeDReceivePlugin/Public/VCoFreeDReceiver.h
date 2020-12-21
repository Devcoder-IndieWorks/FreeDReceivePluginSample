// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "VCoroutineBase.h"

class FREEDRECEIVEPLUGIN_API FVCoFreeDReceiver : public FVCoroutineBase
{
public:
    FVCoFreeDReceiver( float InDelay, TFunction<void( float )>&& InAction, TFunction<void( float )>&& InDelayAction );

protected:
    void Run( float InDeltaTime ) override;

private:
    float Delay;
    TFunction<void( float )> Action;
    TFunction<void( float )> DelayAction;
};
