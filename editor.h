#pragma once

#include "sprite.h"
#include <stddef.h>

typedef struct
{
    char * title;
    Sprite sprite;
}SpriteEditor;

SpriteEditor * sprite_editor_init(char * title,size_t width,size_t height,size_t pixelsize);

void           sprite_editor_render_sprite(SpriteEditor * editor,size_t x,size_t y);

void           sprite_editor_load_sprite(SpriteEditor * editor,const Sprite * sprite);

void           sprite_editor_edit_sprite(SpriteEditor * editor,size_t x,size_t y,uint8_t color);

void           sprite_editor_draw_sprite(SpriteEditor * editor,size_t x,size_t y);
                               
void           sprite_editor_event_loop(SpriteEditor * editor);
                                        
void           sprite_editor_exit(SpriteEditor * editor);
