// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "VCoroutineBase.h"

class FREEDRECEIVEPLUGIN_API FVCoFreeDReceiver : public FVCoroutineBase
{
public:
    FVCoFreeDReceiver( float InDelay, TFunctionRef<void( float )> InAction, TFunctionRef<void( float )> InDelayAction );

protected:
    void Run( float InDeltaTime ) override;

private:
    float Delay;
    TFunctionRef<void( float )> Action;
    TFunctionRef<void( float )> DelayAction;
};
