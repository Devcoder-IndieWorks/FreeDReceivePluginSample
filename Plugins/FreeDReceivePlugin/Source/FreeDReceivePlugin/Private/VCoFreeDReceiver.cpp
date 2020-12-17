#include "VCoFreeDReceiver.h"

FVCoFreeDReceiver::FVCoFreeDReceiver( float InDelay, TFunctionRef<void( float )> InAction, TFunctionRef<void( float )> InDelayAction )
    : Delay( InDelay )
    , Action( InAction )
    , DelayAction( InDelayAction )
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
