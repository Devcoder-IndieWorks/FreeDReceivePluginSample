#include "FreeDSendActor.h"
#include "VFreeDReceiveActor.h"

AFreeDSendActor::AFreeDSendActor( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    Interval = 1.0f;
}

void AFreeDSendActor::BeginPlay()
{
    FreeDDatas.Add( TEXT( "D1 FF CB A0 41 F8 CC F2 FF E1 3F 00 69 2E 00 E5 C4 01 5E AA 01 24 41 01 40 53 00 00 AC" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 46 F8 CC EE FF E1 49 00 69 2E 00 E5 C4 01 5E AA 01 24 41 01 40 52 00 00 A2" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 53 F8 CC DF FF E1 5A 00 69 2E 00 E5 C3 01 5E AA 01 24 40 01 40 53 00 00 94" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 6D F8 CC D4 FF E1 57 00 69 2E 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 87" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 71 F8 CC D2 FF E1 57 00 69 2F 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 84" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 5D F8 CC DD FF E1 50 00 69 2F 00 E5 C3 01 5E AA 01 24 40 01 40 53 00 00 95" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 48 F8 CC EC FF E1 48 00 69 2F 00 E5 C4 01 5E AA 01 24 40 01 40 53 00 00 A2" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 35 F8 CC E6 FF E1 4D 00 69 2F 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 B6" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 38 F8 CC E6 FF E1 55 00 69 2F 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 AB" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 50 F8 CC DC FF E1 4F 00 69 2F 00 E5 C3 01 5E AA 01 24 40 01 40 53 00 00 A4" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 5C F8 CC DF FF E1 53 00 69 2E 00 E5 C3 01 5E A9 01 24 41 01 40 53 00 00 92" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 59 F8 CC DD FF E1 5C 00 69 2E 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 8D" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 48 F8 CC E0 FF E1 62 00 69 2F 00 E5 C3 01 5E AA 01 24 41 01 40 53 00 00 94" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3E F8 CC F2 FF E1 61 00 69 2E 00 E5 C2 01 5E AA 01 24 40 01 40 53 00 00 90" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 43 F8 CC EF FF E1 50 00 69 2E 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 9E" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 42 F8 CC EC FF E1 53 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 9E" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 45 F8 CC D9 FF E1 66 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 9B" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 4E F8 CC CB FF E1 6D 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 99" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 5E F8 CC CB FF E1 63 00 69 2F 00 E5 C3 01 5E AA 01 24 40 01 40 53 00 00 93" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 59 F8 CC D8 FF E1 5F 00 69 2F 00 E5 C3 01 5E AA 01 24 41 01 40 52 00 00 8F" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 50 F8 CC E5 FF E1 5A 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 90" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 40 F8 CC E8 FF E1 5E 00 69 2F 00 E5 C2 01 5E AA 01 24 40 01 40 53 00 00 9A" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 34 F8 CC EA FF E1 62 00 69 2F 00 E5 C2 01 5E AA 01 24 40 01 40 52 00 00 A1" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3D F8 CC DB FF E1 65 00 69 2F 00 E5 C2 01 5E AA 01 24 40 01 40 53 00 00 A3" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 50 F8 CC D6 FF E1 6B 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 52 00 00 8F" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 53 F8 CC D4 FF E1 6E 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 52 00 00 8B" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 4F F8 CC DD FF E1 6A 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 89" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 46 F8 CC EF FF E1 64 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 86" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3D F8 CC F7 FF E1 63 00 69 2E 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 89" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3C F8 CC FE FF E1 5E 00 69 2E 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 88" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 45 F8 CC EE FF E1 60 00 69 2E 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 8D" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 49 F8 CC E0 FF E1 6C 00 69 2E 00 E5 C1 01 5E AA 01 24 41 01 40 53 00 00 8C" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 4C F8 CC C8 FF E1 6D 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 9E" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 49 F8 CC DA FF E1 6A 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 92" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 45 F8 CC E2 FF E1 6A 00 69 2F 00 E5 C2 01 5E AA 01 24 40 01 40 53 00 00 8F" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3F F8 CC ED FF E1 5E 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 95" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3D F8 CC F2 FF E1 59 00 69 2F 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 97" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 4B F8 CC E4 FF E1 5B 00 69 2E 00 E5 C3 01 5E AA 01 24 40 01 40 53 00 00 96" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 52 F8 CC DD FF E1 62 00 69 2E 00 E5 C2 01 5E AB 01 24 40 01 40 52 00 00 90" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 51 F8 CC D2 FF E1 6C 00 69 2E 00 E5 C2 01 5E AB 01 24 41 01 40 53 00 00 90" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 53 F8 CC D8 FF E1 63 00 69 2E 00 E5 C2 01 5E AA 01 24 41 01 40 53 00 00 92" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3F F8 CC E4 FF E1 67 00 69 2E 00 E5 C2 01 5E AB 01 24 41 01 40 52 00 00 96" ) );
    FreeDDatas.Add( TEXT( "D1 FF CB A0 3F F8 CC E6 FF E1 64 00 69 2F 00 E5 C2 01 5E AB 01 24 41 01 40 53 00 00 95" ) );

    Super::BeginPlay();
}

void AFreeDSendActor::StartSend( AVFreeDReceiveActor* InFreeDReceiver )
{
    FreeDReceiver = InFreeDReceiver;
    DataCounter = 0;

    GetWorld()->GetTimerManager().SetTimer( TimerHandle, [this]{
        if ( DataCounter >= FreeDDatas.Num() )
            DataCounter = 0;

        auto msg = FreeDDatas[ DataCounter++ ];
        FreeDReceiver->ReceiveMessage( msg );
    }, Interval, true );

    FreeDReceiver->StartReceive();
}

void AFreeDSendActor::StopSend()
{
    GetWorld()->GetTimerManager().ClearTimer( TimerHandle );
}
