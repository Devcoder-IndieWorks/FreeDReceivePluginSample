#include "VFreeDReceiveActor.h"
#include "VCoFreeDReceiver.h"
#include "VFreeDHelper.h"
#include "VLog.h"

AVFreeDReceiveActor::AVFreeDReceiveActor( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

void AVFreeDReceiveActor::BeginPlay()
{
    TWeakObjectPtr<AVFreeDReceiveActor> weakPtr = this;
    Coroutine = MakeShareable( new FVCoFreeDReceiver( Delay, 
        [weakPtr]( float delta ){
            if ( weakPtr->ReceiveQueue.IsEmpty() )
                return;

            FString msg;
            weakPtr->ReceiveQueue.Dequeue( msg );
            if ( msg.IsEmpty() )
                return;

            weakPtr->IntermediateQueue.Enqueue( msg );
        }, 
        [weakPtr]( float delta ){
            if ( weakPtr->IntermediateQueue.IsEmpty() )
                return;

            FString msg;
            weakPtr->IntermediateQueue.Dequeue( msg );
            if ( msg.IsEmpty() )
                return;

            if ( !weakPtr->OnFreeDReceiveEventDelegate.IsBound() )
                return;

            FRotator rot;
            rot.Roll = VFreeDHelper::GetRotationValueFromHexString( msg, 2, 4 );
            rot.Pitch = VFreeDHelper::GetRotationValueFromHexString( msg, 5, 7 );
            rot.Yaw = VFreeDHelper::GetRotationValueFromHexString( msg, 8, 10 );

            FVector loc;
            loc.X = VFreeDHelper::GetLocationValueFromHexString( msg, 11, 13 );
            loc.Y = VFreeDHelper::GetLocationValueFromHexString( msg, 14, 16 );
            loc.Z = VFreeDHelper::GetLocationValueFromHexString( msg, 17, 19 );

            FVector2D zoomFocus;
            zoomFocus.X = VFreeDHelper::GetZoomFocusValueFromHexString( msg, 20, 22 );
            zoomFocus.Y = VFreeDHelper::GetZoomFocusValueFromHexString( msg, 23, 25 );

            weakPtr->OnFreeDReceiveEventDelegate.Broadcast( rot, loc, zoomFocus );
        } ) );

    if ( Coroutine.IsValid() )
        Coroutine->Start();

    TickerHandle = FTicker::GetCoreTicker().AddTicker( FTickerDelegate::CreateLambda(
        [weakPtr]( float delta ){
            if ( weakPtr->Coroutine.IsValid() ) {
                weakPtr->Coroutine->Execute( delta );
                return true;
            }
            return false;
        } ) );

    Super::BeginPlay();
}

void AVFreeDReceiveActor::EndPlay( const EEndPlayReason::Type InEndPlayReason )
{
    if ( Coroutine.IsValid() )
        Coroutine->Stop();

    if ( TickerHandle.IsValid() )
        FTicker::GetCoreTicker().RemoveTicker( TickerHandle );

    Coroutine.Reset();

    Super::EndPlay( InEndPlayReason );
}

void AVFreeDReceiveActor::ReceiveMessage( const FString& InMessage )
{
    if ( !InMessage.IsEmpty() )
        ReceiveQueue.Enqueue( InMessage );
}
