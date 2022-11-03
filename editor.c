#include "editor.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

SDL_Window  * g_sdl_window  = NULL;
SDL_Surface * g_sdl_surface = NULL;
SDL_Event     g_sdl_event;

#define ATSIN_RGBA(r,g,b,a) (a << 24) | ( r << 16) | ( g << 8) | ( b << 0) 
#define ATSIN_RGB(r,g,b) ATSIN_RGBA(r,g,b,0)

void drawpixel(size_t x,size_t y,uint32_t pixel)
{
    size_t offset = y * g_sdl_surface->pitch + x *
                        g_sdl_surface->format->BytesPerPixel;
    
     uint32_t * tmp = (uint32_t*)((uint8_t*)g_sdl_surface->pixels + offset);
     *tmp = pixel;
}
void fill_rectangle(size_t x,size_t y,size_t width,size_t height,uint32_t pixel)
{
    for(size_t posy = 0;posy < height;++posy)
    {
        for(size_t posx = 0;posx < width;++posx)
        {           
           drawpixel(posx+x,posy+y,pixel);
        }
    }
}

SpriteEditor * sprite_editor_init(char * title,size_t width,size_t height,size_t pixelsize)
{
    SpriteEditor * editor = malloc(sizeof(SpriteEditor));
    editor->title = malloc(sizeof(char) * strlen(title));
    strcpy(editor->title,title);
    
    editor->sprite.width     = width;
    editor->sprite.height    = height;
    editor->sprite.pixelsize = pixelsize;
    editor->sprite.pixels    = malloc(width*height);
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        puts(SDL_GetError());
        exit(1);
    }
    
    g_sdl_window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,
                                    height,
                                    SDL_WINDOW_SHOWN);
    if(g_sdl_window == NULL)
    {
        puts(SDL_GetError());
        exit(1);
    }
    
    g_sdl_surface = SDL_GetWindowSurface(g_sdl_window);
    
    if(g_sdl_surface == NULL)
    {
        puts(SDL_GetError());
        exit(1);
    }    
    
    return editor;
}
void sprite_editor_load_sprite(SpriteEditor * editor,const Sprite * sprite)
{
    for(size_t posy = 0;posy < sprite->height;++posy)
    {
        for(size_t posx = 0;posx < sprite->width;++posx)
        {            
            size_t sprite_offset = (posx * sprite->width) + posy;
            size_t editor_offset = (posx * editor->sprite.width) + posy;
            
            editor->sprite.pixels[editor_offset] = sprite->pixels[sprite_offset];
        }
    }
}
void sprite_editor_render_sprite(SpriteEditor * editor,size_t x,size_t y)
{
    fill_rectangle(0,0,editor->sprite.width,
                       editor->sprite.height,
                       ATSIN_RGB(0,0,0));
    
    for(size_t posy = 0;posy < editor->sprite.height;++posy)
    {
        for(size_t posx = 0;posx < editor->sprite.width;++posx)
        {
            size_t offset = (posx * editor->sprite.width) + posy;
            
            uint32_t color = editor->sprite.pixels[offset] == 1 ? ATSIN_RGB(255,255,255) :
                                                                  ATSIN_RGB(0,0,0);
            drawpixel(x+posx,y+posy,color);
        }
    }
}
void sprite_editor_edit_sprite(SpriteEditor * editor,size_t x,size_t y,uint8_t color)
{
    size_t offset = (x * editor->sprite.width) + y;
    editor->sprite.pixels[offset] = color;
}
void sprite_editor_draw_sprite(SpriteEditor * editor,size_t x,size_t y)
{
    fill_rectangle(0,0,editor->sprite.width,
                       editor->sprite.height,
                       ATSIN_RGB(0,0,0));

    for(size_t posy = 0;posy < editor->sprite.height/editor->sprite.pixelsize;++posy)
    {
        for(size_t posx = 0;posx < editor->sprite.width/editor->sprite.pixelsize;++posx)
        { 
            size_t offset = (posx * editor->sprite.width) + posy;
            
            if(editor->sprite.pixels[offset] == 1)
            {
                fill_rectangle(x+posx*editor->sprite.pixelsize,
                               y+posy*editor->sprite.pixelsize,
                               editor->sprite.pixelsize-1,
                               editor->sprite.pixelsize-1,
                               ATSIN_RGB(255,255,255));
            }
            else
            {
            
                fill_rectangle(x+posx*editor->sprite.pixelsize,
                               y+posy*editor->sprite.pixelsize,
                               editor->sprite.pixelsize-1,
                               editor->sprite.pixelsize-1,
                               ATSIN_RGB(0,0,0));
            
            }
        }
    }
}
void sprite_editor_event_loop(SpriteEditor * editor)
{
    bool is_running = 1;
    while(is_running)
    {
        while(SDL_PollEvent(&g_sdl_event))
        {
            switch(g_sdl_event.type)
            {
                case SDL_QUIT:
                    is_running = false;
                break;
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                   //g_sdl_event.key.keysym.scancode
                break;
            }
        }
        SDL_UpdateWindowSurface(g_sdl_window);
    }
}

void sprite_editor_exit(SpriteEditor * editor)
{
    free(editor->sprite.pixels);
    free(editor->title);
    free(editor);
    
    SDL_DestroyWindow(g_sdl_window);
}
