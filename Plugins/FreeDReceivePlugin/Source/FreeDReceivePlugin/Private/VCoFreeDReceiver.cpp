#include "VCoFreeDReceiver.h"

FVCoFreeDReceiver::FVCoFreeDReceiver( float InDelay, TFunction<void( float )>&& InAction, TFunction<void( float )>&& InDelayAction )
    : Delay( InDelay )
    , Action( MoveTemp( InAction ) )
    , DelayAction( MoveTemp( InDelayAction ) )
{
}

void FVCoFreeDReceiver::Run( float InDeltaTime )
{
    if ( IsActivate() )
        Action( InDeltaTime );

    VCoroutineReEnter( this ) {
    VCoroutineEntry:
        VCoYieldReturnWait( Delay );

        while( true ) {
            if ( IsActivate() )
                DelayAction( InDeltaTime );
            else
                break;
            VCoYieldReturnWait( Delay );
        }
    }
}
