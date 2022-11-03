#pragma once

#include "sprite.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    char   * name;
    char   * author;
    uint8_t  version;
    bool     auto_save;
    bool     modified;
    Sprite   sprite;
}SpriteDoc;

enum SpriteDocVersion{
    SPRITE_DOC_01_VERSION,
};

SpriteDoc sprite_doc_create(char * name,char * author,size_t width,size_t height,uint8_t pixelsize);
SpriteDoc sprite_doc_create_from_sprite(char * name,char * author,const Sprite * sprite);
SpriteDoc sprite_doc_load(const char * filename);
void      sprite_doc_save(const SpriteDoc * doc);
void      sprite_doc_clearup(SpriteDoc * doc);
