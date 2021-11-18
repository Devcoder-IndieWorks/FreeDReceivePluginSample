#include "VFreeDReceiveActor.h"
#include "VCoFreeDReceiver.h"
#include "VFreeDHelper.h"
#include "VLog.h"

AVFreeDReceiveActor::AVFreeDReceiveActor( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    Delay = 1.0f;
    Interval = 1.0f;
}

void AVFreeDReceiveActor::BeginPlay()
{
    Super::BeginPlay();

    Coroutine = MakeShareable( new FVCoFreeDReceiver( Delay, Interval, 
        [this]( float delta ){
            if ( ReceiveQueue.IsEmpty() )
                return;

            FString msg;
            ReceiveQueue.Dequeue( msg );
            if ( msg.IsEmpty() )
                return;

            IntermediateQueue.Enqueue( msg );
            VCLOG( OutputLog, Log, TEXT( "#### Receiver Queue ---> Intermediate Queue. ####" ) );
        }, 
        [this]( float elapsedTime ){
            if ( IntermediateQueue.IsEmpty() )
                return;

            FString msg;
            IntermediateQueue.Dequeue( msg );
            if ( msg.IsEmpty() )
                return;

            if ( OnFreeDReceiveMessageDelegate.IsBound() ) {
                OnFreeDReceiveMessageDelegate.Broadcast( msg );
                VCLOG( OutputLog, Log, TEXT( "#### Delay received FreeD data. Elapsed time:[%f] ####" ), elapsedTime );
                return;
            }

            if ( OnFreeDReceiveEventDelegate.IsBound() ) {
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
			    
                OnFreeDReceiveEventDelegate.Broadcast( rot, loc, zoomFocus );
                VCLOG( OutputLog, Log, TEXT( "#### Delay received FreeD data. Elapsed time:[%f] ####" ), elapsedTime );
                return;
            }
        }, 
        [this]{
            return GetWorld()->GetTimeSeconds();
        } ) );

    TickerHandle = FTicker::GetCoreTicker().AddTicker( FTickerDelegate::CreateLambda(
        [this]( float delta ){
            if ( Coroutine.IsValid() ) {
                Coroutine->Execute( delta );
                return true;
            }
            return false;
        } ) );
}

void AVFreeDReceiveActor::EndPlay( const EEndPlayReason::Type InEndPlayReason )
{
    StopReceive();

    if ( TickerHandle.IsValid() )
        FTicker::GetCoreTicker().RemoveTicker( TickerHandle );

    Coroutine.Reset();

    Super::EndPlay( InEndPlayReason );
}

void AVFreeDReceiveActor::ReceiveMessage( const FString& InMessage )
{
    if ( !InMessage.IsEmpty() ) {
        ReceiveQueue.Enqueue( InMessage );
        VCLOG( OutputLog, Log, TEXT( "#### Receive Queue ####" ) );
    }
}

void AVFreeDReceiveActor::StartReceive()
{
    if ( Coroutine.IsValid() ) {
        if ( Coroutine->IsActivate() )
            StopReceive();

        Coroutine->Start();
    }
}

void AVFreeDReceiveActor::StopReceive()
{
    if ( Coroutine.IsValid() )
        Coroutine->Stop();
}
