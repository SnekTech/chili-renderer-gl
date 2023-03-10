//
// Created by syf on 2023/3/8.
//

#ifndef CHILI_RENDERER_GL_SURFACE_H
#define CHILI_RENDERER_GL_SURFACE_H

#include "Colors.h"
#include <string>
#include <memory>
#include <assert.h>

class Surface
{
public:
    Surface( unsigned int width,unsigned int height,unsigned int pitch )
            :
            pBuffer( std::make_unique<Color[]>( pitch * height ) ),
            width( width ),
            height( height ),
            pitch( pitch )
    {}
    Surface( unsigned int width,unsigned int height )
            :
            Surface( width,height,width )
    {}
    Surface( Surface&& source ) noexcept
            :
            pBuffer( std::move( source.pBuffer ) ),
            width( source.width ),
            height( source.height ),
            pitch( source.pitch )
    {}
    Surface( Surface& ) = delete;
    Surface& operator=( Surface&& donor ) noexcept
    {
        width = donor.width;
        height = donor.height;
        pitch = donor.pitch;
        pBuffer = std::move( donor.pBuffer );
        donor.pBuffer = nullptr;
        return *this;
    }
    Surface& operator=( const Surface& ) = delete;
    ~Surface() = default;
    void Clear( const Color& fillValue  )
    {
//        wmemset(reinterpret_cast<wchar_t *>(pBuffer.get()), fillValue.dword, pitch * height * sizeof( Color ) );
        for (auto y = 0; y < height; y++)
        {
            for (auto x = 0; x < pitch; x++)
            {
                PutPixel(x, y, fillValue);
            }
        }
    }

    void PutPixel( unsigned int x,unsigned int y,const Color& color )
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < width );
        assert( y < height );
        pBuffer[y * pitch + x] = color;
    }
    void PutPixelAlpha( unsigned int x,unsigned int y,const Color& color );
    [[nodiscard]] Color GetPixel( unsigned int x,unsigned int y ) const
    {
        assert( x >= 0 );
        assert( y >= 0 );
        assert( x < width );
        assert( y < height );
        return pBuffer[y * pitch + x];
    }
    [[nodiscard]] unsigned int GetWidth() const
    {
        return width;
    }
    [[nodiscard]] unsigned int GetHeight() const
    {
        return height;
    }
    [[nodiscard]] unsigned int GetPitch() const
    {
        return pitch;
    }
    Color* GetBufferPtr()
    {
        return pBuffer.get();
    }
    [[nodiscard]] const Color* GetBufferPtrConst() const
    {
        return pBuffer.get();
    }
    void Copy( const Surface& src );
private:
    // calculate pixel pitch required for given byte alignment (must be multiple of 4 bytes)
    static unsigned int GetPitch( unsigned int width,unsigned int byteAlignment )
    {
        assert( byteAlignment % 4 == 0 );
        const unsigned int pixelAlignment = byteAlignment / sizeof( Color );
        return width + ( pixelAlignment - width % pixelAlignment ) % pixelAlignment;
    }
    Surface( unsigned int width,unsigned int height,unsigned int pitch,std::unique_ptr<Color[]> pBufferParam )
            :
            width( width ),
            height( height ),
            pBuffer( std::move( pBufferParam ) ),
            pitch( pitch )
    {}
private:
    std::unique_ptr<Color[]> pBuffer;
    unsigned int width;
    unsigned int height;
    unsigned int pitch; // pitch is in PIXELS, not bytes!
};

#endif //CHILI_RENDERER_GL_SURFACE_H
