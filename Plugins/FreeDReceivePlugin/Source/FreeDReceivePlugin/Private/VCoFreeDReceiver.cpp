#include "VCoFreeDReceiver.h"
#include "VLog.h"

FVCoFreeDReceiver::FVCoFreeDReceiver( float InDelay, float InInterval, TFunction<void( float )>&& InAction, TFunction<void( float )>&& InDelayAction, 
    TFunction<float( void )>&& InWorldTimeSeconds )
    : Delay( InDelay )
    , Interval( InInterval )
    , PrevTime( 0.0f )
    , Action( MoveTemp( InAction ) )
    , DelayAction( MoveTemp( InDelayAction ) )
    , WorldTimeSeconds( MoveTemp( InWorldTimeSeconds ) )
{
}

void FVCoFreeDReceiver::Run( float InDeltaTime )
{
    if ( !IsActivate() )
        return;
    
    Action( InDeltaTime );

    VCoroutineReEnter( this ) {
    VCoroutineEntry:
        VCoYieldReturnWait( Delay );
        PrevTime = WorldTimeSeconds();
        //VLOG( Log, TEXT( "#### First delay. Delay time: [%f], Prev time: [%f] ####" ), Delay, PrevTime );

        while( true ) {
            {
                float curTime = WorldTimeSeconds();
                //VLOG( Log, TEXT( "#### Current time: [%f], Prev time: [%f]" ), curTime, PrevTime );
                float elapsedTime = curTime - PrevTime;
                PrevTime = curTime;
			    
                if ( IsActivate() )
                    DelayAction( elapsedTime );
                else
                    break;
            }
            VCoYieldReturnWait( Interval );
        }
    }
}
