//
// Created by syf on 2023/3/8.
//

#include "Surface.h"

void Surface::PutPixelAlpha( unsigned int x,unsigned int y,const Color& color )
{
    assert( x >= 0 );
    assert( y >= 0 );
    assert( x < width );
    assert( y < height );
    // load source pixel
    const Color d = GetPixel( x,y );

    // blend channels
    const unsigned char resultRed = (color.GetR() * color.GetA() + d.GetR() * (255u - color.GetA())) / 256u;
    const unsigned char resultGreen = (color.GetG() * color.GetA() + d.GetG() * (255u - color.GetA())) / 256u;
    const unsigned char resultBlue = (color.GetB() * color.GetA() + d.GetB() * (255u - color.GetA())) / 256u;

    // pack channels back into pixel and fire pixel onto surface
    PutPixel( x,y,{resultRed, resultGreen, resultBlue } );
}

void Surface::Copy( const Surface & src )
{
    assert( width == src.width );
    assert( height == src.height );

    for(auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < pitch; x++)
        {
            pBuffer[y * pitch + x] = src.GetPixel(x, y);
        }
    }
}
