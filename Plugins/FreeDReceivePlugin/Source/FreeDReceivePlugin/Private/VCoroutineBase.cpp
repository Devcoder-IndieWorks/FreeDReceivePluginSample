#include "VCoroutineBase.h"
#include "VLog.h"

void FVCoroutineBase::Start()
{
    Activate = true;
}

void FVCoroutineBase::Stop()
{
    Activate = false;
    Reset();
}

void FVCoroutineBase::Reset()
{
    Line = 0;
    WaitTime = 0.0f;
}

void FVCoroutineBase::Execute( float InDeltaTime )
{
    CalcWaitTime( InDeltaTime );
    Run( InDeltaTime );
}

void FVCoroutineBase::CalcWaitTime( float InDeltaTime )
{
    WaitTime -= InDeltaTime;
}

void FVCoroutineBase::Run( float InDeltaTime )
{
    VLOG( Warning, TEXT( "Not implementation." ) );
}

bool FVCoroutineBase::IsActivate() const
{
    return Activate;
}

//-----------------------------------------------------------------------------

FVCoroutineRef::FVCoroutineRef( FVCoroutineBase& InCoroutine )
    : LineRef( InCoroutine.Line )
    , WaitTimeRef( InCoroutine.WaitTime )
{
}

FVCoroutineRef::FVCoroutineRef( FVCoroutineBase* InCoroutine )
    : LineRef( InCoroutine->Line )
    , WaitTimeRef( InCoroutine->WaitTime )
{
}

FVCoroutineRef::operator uint32() const
{
    return LineRef;
}

uint32 FVCoroutineRef::operator=( uint32 InLine )
{
    return (LineRef = InLine);
}

void FVCoroutineRef::SetWaitTime( float InWaitTime )
{
    WaitTimeRef = InWaitTime;
}

bool FVCoroutineRef::HasWaiting() const
{
    return WaitTimeRef > 0.0f;
}
