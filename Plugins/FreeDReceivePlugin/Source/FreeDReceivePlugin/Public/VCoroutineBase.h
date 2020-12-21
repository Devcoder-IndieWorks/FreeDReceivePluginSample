// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

class FREEDRECEIVEPLUGIN_API FVCoroutineBase
{
public:
    FVCoroutineBase();
    virtual ~FVCoroutineBase() = default;

    void Start();
    void Stop();
    void Execute( float InDeltaTime );

    bool IsActivate() const;

protected:
    virtual void Run( float InDeltaTime );

private:
    void Reset();
    void CalcWaitTime( float InDeltaTime );

private:
    friend class FVCoroutineRef;

    uint32 Line;
    float WaitTime;
    bool Activate;
};

//-----------------------------------------------------------------------------

class FREEDRECEIVEPLUGIN_API FVCoroutineRef
{
public:
    FVCoroutineRef( FVCoroutineBase& InCoroutine );
    FVCoroutineRef( FVCoroutineBase* InCoroutine );

    operator uint32() const;
    uint32 operator=( uint32 InLine );

    void SetWaitTime( float InWaitTime );
    bool HasWaiting() const;

private:
    uint32& LineRef;
    float& WaitTimeRef;
};

//-----------------------------------------------------------------------------

#define VCoroutineReEnter( c ) switch( FVCoroutineRef CoRef = c )
#define VCoroutineEntry \
BailOutOfCoroutine: return;\
case 0

//-----------------------------------------------------------------------------

#define VCoroutineBegin \
for ( bool flag = false; ; flag = !flag ) {\
    if ( flag )\
        goto BailOutOfCoroutine;\
    else
#define VCoroutineEnd }

#define VCoroutineNull( wait )                 VCoroutineBegin wait; VCoroutineEnd
#define VCoroutineStatement( wait, statement ) VCoroutineBegin { wait; statement; } VCoroutineEnd

//-----------------------------------------------------------------------------

#define VCoYieldNullBegin \
if ( (CoRef = __LINE__) == 0 ) {\
    case __LINE__: ;\
}\
else {
#define VCoYieldNullEnd }

#define VCoYieldReturnNull                       VCoYieldNullBegin VCoroutineNull( CoRef.SetWaitTime(0.0f) ) VCoYieldNullEnd
#define VCoYieldReturnNullStatement( statement ) VCoYieldNullBegin VCoroutineStatement( CoRef.SetWaitTime(0.0f), statement ) VCoYieldNullEnd

//-----------------------------------------------------------------------------

#define VCoYieldWaitBegin \
if ( (CoRef = __LINE__) == 0 ) {\
    case __LINE__:\
        if (CoRef.HasWaiting())\
            goto BailOutOfCoroutine;\
}\
else {
#define VCoYieldWaitEnd }

#define VCoYieldReturnWait( wait )                     VCoYieldWaitBegin VCoroutineNull( CoRef.SetWaitTime(wait) ) VCoYieldWaitEnd
#define VCoYieldReturnWaitStatement( wait, statement ) VCoYieldWaitBegin VCoroutineStatement( CoRef.SetWaitTime(wait), statement ) VCoYieldWaitEnd
