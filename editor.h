#pragma once

#include "sprite_doc.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    SpriteDoc doc;
    
    struct Window
    {
        size_t width;
        size_t height; 
    }window;
    
    struct SpriteDocWindow
    {
        size_t x;
        size_t y;
        size_t width;
        size_t height;
    }sprite_doc_window;

    struct ColorsWindow
    {
        size_t x;
        size_t y;
        size_t width;
        size_t height;
    }colors_window;

    struct Cursor
    {
        size_t   x;
        size_t   y;
        size_t   pixelsize;
        uint32_t color;
        uint8_t  selected;
    }cursor;

    bool edit_mode;
    bool color_mode;
}SpriteEditor;

SpriteEditor * sprite_editor_init(char * title,size_t width,size_t height,size_t pixelsize);

void           sprite_editor_render_sprite(SpriteEditor * editor,size_t x,size_t y);

void           sprite_editor_load_sprite_from_disk(SpriteEditor * editor,const char * filename);

void           sprite_editor_load_sprite(SpriteEditor * editor,const SpriteDoc * doc);

void           sprite_editor_edit_sprite(SpriteEditor * editor,size_t x,size_t y,uint8_t color);

void           sprite_editor_draw_sprite(SpriteEditor * editor,size_t x,size_t y);
                               
void           sprite_editor_event_loop(SpriteEditor * editor);
                                        
void           sprite_editor_exit(SpriteEditor * editor);
