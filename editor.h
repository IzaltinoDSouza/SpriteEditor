#pragma once

#include "sprite.h"
#include <stddef.h>

typedef struct
{
    char * title;
    Sprite sprite;
    struct Events
    {
        struct KeyboardEvent
        {
            void (*keydown_callback)(uint16_t scancode);
            void (*keyup_callback)(uint16_t scancode);
        }kb_event;
        struct MouseEvent
        {
            void (*move_callback)(int x,int y,
                                  int relative_x,int relative_y,
                                  uint32_t state);
                                  
            void (*button_callback)(int x,int y,
                                    uint8_t button,
                                    uint8_t state,
                                    uint8_t clicks);
                                    
            void (*wheel_callback)(int x,int y,
                                   float precise_x,float precise_y,
                                   uint32_t direction);
            
        }mouse_event;
    }events;
}SpriteEditor;

SpriteEditor * sprite_editor_init(char * title,size_t width,size_t height,size_t pixelsize);

void           sprite_editor_render_sprite(SpriteEditor * editor,size_t x,size_t y);

void           sprite_editor_load_sprite(SpriteEditor * editor,const Sprite * sprite);

void           sprite_editor_edit_sprite(SpriteEditor * editor,size_t x,size_t y,uint8_t color);

void           sprite_editor_draw_sprite(SpriteEditor * editor,size_t x,size_t y);

void           sprite_editor_keyup_event(SpriteEditor * editor,
                                         void (*callback)(uint16_t scancode));
                               
void           sprite_editor_keydown_event(SpriteEditor * editor,
                                           void (*callback)(uint16_t scancode));

void           sprite_editor_mouse_move_event(SpriteEditor * editor,
                                           void (*callback)(int x,int y,
                                                            int relative_x,int relative_y,
                                                            uint32_t state));
                                           
void           sprite_editor_mouse_button_event(SpriteEditor * editor,
                                           void (*callback)(int x,int y,
                                                            uint8_t button,
                                                            uint8_t state,
                                                            uint8_t clicks));
                                           
void           sprite_editor_mouse_wheel_event(SpriteEditor * editor,
                                           void (*callback)(int x,int y,
                                                            float precise_x,float precise_y,
                                                            uint32_t direction));
                               
void           sprite_editor_event_loop(SpriteEditor * editor);
                                        
void           sprite_editor_exit(SpriteEditor * editor);
