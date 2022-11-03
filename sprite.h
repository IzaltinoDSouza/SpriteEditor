#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t  width;
    uint32_t  height;
    uint8_t   pixelsize;
    uint8_t * pixels;
}Sprite;

Sprite sprite_load(char * filename);
void   sprite_save(char * filename,const Sprite * sprite);
