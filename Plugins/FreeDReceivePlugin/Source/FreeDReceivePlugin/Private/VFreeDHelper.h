// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

namespace VFreeDHelper
{
    float GetRotationValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd );
    float GetLocationValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd );
    float GetZoomFocusValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd );

    //-------------------------------------------------------------------------

    TArray<uint8> GetByteArrayFromHexString( const FString& InHexString, int32 InStart, int32 InEnd );
    int32 Interpret24bitAsInt32( const TArray<uint8>& InByteArray );
    float ConvertRotationValue( int32 InValue );
    float ConvertLocationValue( int32 InValue );
}
