#include "VFreeDHelper.h"

namespace VFreeDHelper
{
    static const float MultiplyRotationValue = 1.0f/32768.0f;
    static const float MultiplyLocationValue = 1.0f/640.0f;

    float GetRotationValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd )
    {
        auto byteArray = GetByteArrayFromHexString( InHexString, InStart, InEnd );
        auto value = Interpret24bitAsInt32( byteArray );
        return ConvertRotationValue( value );
    }

    float GetLocationValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd )
    {
        auto byteArray = GetByteArrayFromHexString( InHexString, InStart, InEnd );
        auto value = Interpret24bitAsInt32( byteArray );
        return ConvertLocationValue( value );
    }

    float GetZoomFocusValueFromHexString( const FString& InHexString, int32 InStart, int32 InEnd )
    {
        auto byteArray = GetByteArrayFromHexString( InHexString, InStart, InEnd );
        return (float)Interpret24bitAsInt32( byteArray );
    }

    TArray<uint8> GetByteArrayFromHexString( const FString& InHexString, int32 InStart, int32 InEnd )
    {
        TArray<FString> hexCodes;
        InHexString.ParseIntoArray( hexCodes, TEXT( " " ) );

        TArray<uint8> byteArray;
        for ( int32 idx = InStart; idx <= InEnd; idx++ ) {
            uint8 number = FParse::HexNumber( *(hexCodes[ idx ]) );
            byteArray.Add( number );
        }

        return byteArray;
    }

    int32 Interpret24bitAsInt32( const TArray<uint8>& InByteArray )
    {
        int32 number = 0x00;
        if ( (InByteArray[0] & 0x80) > 0 )
            number = 0xFF;

        number = (number << 8) | InByteArray[0];
        number = (number << 8) | InByteArray[1];
        number = (number << 8) | InByteArray[2];

        return number;
    }

    float ConvertRotationValue( int32 InValue )
    {
        return (float)InValue * MultiplyRotationValue;
    }

    float ConvertLocationValue( int32 InValue )
    {
        return (float)InValue * MultiplyLocationValue;
    }
}
