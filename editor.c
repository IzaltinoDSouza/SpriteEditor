#include "editor.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

SDL_Window  * g_sdl_window  = NULL;
SDL_Surface * g_sdl_surface = NULL;
SDL_Event     g_sdl_event;

#define ATSIN_RGBA(r,g,b,a) (a << 24) | ( r << 16) | ( g << 8) | ( b << 0) 
#define ATSIN_RGB(r,g,b) ATSIN_RGBA(r,g,b,0)

#include "colors_table.h"

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
void drawcursor(SpriteEditor * editor,size_t x,size_t y,uint32_t color)
{
    fill_rectangle(x+editor->cursor.x * editor->cursor.pixelsize,
                   y+editor->cursor.y * editor->cursor.pixelsize,
                   editor->cursor.pixelsize-1,
                   editor->cursor.pixelsize-1,
                   colors_table[color]);
}
void on_keypressed(SpriteEditor * editor,uint16_t scancode)
{
    if(scancode == SDL_SCANCODE_R)
    {
        sprite_editor_render_sprite(editor,0,0);
    }
    else if(scancode == SDL_SCANCODE_D)
    {
        sprite_editor_draw_sprite(editor,0,0);
    }else if(scancode == SDL_SCANCODE_E)
    {
        editor->cursor.selected = 0;
        editor->cursor.x = 0;
        editor->cursor.y = 0;
        editor->edit_mode = (editor->edit_mode == 1) ? 0 : 1;
        if(editor->edit_mode)
        {
            editor->cursor.pixelsize = editor->doc.sprite.pixelsize;
            editor->color_mode = 0;
        }

    }else if(scancode == SDL_SCANCODE_I)
    {
        editor->cursor.selected = 0;
        editor->cursor.x = 0;
        editor->cursor.y = 0;
        editor->color_mode = (editor->color_mode == 1) ? 0 : 1;
        if(editor->color_mode)
        {
            editor->cursor.pixelsize = 16;
            editor->edit_mode = 0;
        }

    }else if(scancode == SDL_SCANCODE_S)
    {
       sprite_doc_save(&editor->doc);
    }
    /*else
    {
        printf("%d\n",scancode);
    }*/

    if(editor->edit_mode == 1 || editor->color_mode == 1)
    {
        if(scancode == SDL_SCANCODE_LEFT)       editor->cursor.x -= 1;
        else if(scancode == SDL_SCANCODE_RIGHT) editor->cursor.x += 1;
        else if(scancode == SDL_SCANCODE_UP)    editor->cursor.y -= 1;
        else if(scancode == SDL_SCANCODE_DOWN)  editor->cursor.y += 1;

        else if(scancode == SDL_SCANCODE_SPACE)
        {
            editor->cursor.selected = (editor->cursor.selected == 1) ? 0 : 1;
        }

        if(editor->cursor.x == -1) editor->cursor.x = 0;
        if(editor->cursor.y == -1) editor->cursor.y = 0;
    }

    if(editor->edit_mode == 1)
    {
        const size_t x_max = (editor->sprite_doc_window.width  / editor->doc.sprite.pixelsize) - 1;
        const size_t y_max = (editor->sprite_doc_window.height / editor->doc.sprite.pixelsize) - 1;

        if(editor->cursor.x > x_max) editor->cursor.x = x_max;
        if(editor->cursor.y > y_max) editor->cursor.y = y_max;
    }

    if(editor->color_mode == 1)
    {
        const size_t x_max = (editor->colors_window.width  / editor->doc.sprite.pixelsize) - 1;
        const size_t y_max = (editor->colors_window.height / editor->doc.sprite.pixelsize) - 1;

        if(editor->cursor.x > x_max) editor->cursor.x = x_max;
        if(editor->cursor.y > y_max) editor->cursor.y = y_max;
    }
}

SpriteEditor * sprite_editor_init(char * title,size_t width,size_t height,size_t pixelsize)
{
    SpriteEditor * editor = malloc(sizeof(SpriteEditor));
    editor->title = malloc(sizeof(char) * strlen(title));
    strcpy(editor->title,title);

    editor->window.width = width;
    editor->window.width = height;

    editor->sprite_doc_window.x      = 0;
    editor->sprite_doc_window.y      = 0;
    editor->sprite_doc_window.width  = width;
    editor->sprite_doc_window.height = height-100;

    editor->colors_window.x          = 0;
    editor->colors_window.y          = editor->sprite_doc_window.height;
    editor->colors_window.width      = width;
    editor->colors_window.height     = 100;

    editor->doc = sprite_doc_create("untitled","unknown",width,height-100,pixelsize);

    editor->cursor.x         = 0;
    editor->cursor.y         = 0;
    editor->cursor.pixelsize = editor->doc.sprite.pixelsize;
    editor->cursor.selected  = 0;
    editor->cursor.color     = 15;
    editor->edit_mode        = 0;
    editor->color_mode       = 0;
    
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
void sprite_editor_load_sprite_from_disk(SpriteEditor * editor,const char * filename)
{
    if(editor->doc.name)          free(editor->doc.name);
    if(editor->doc.author)        free(editor->doc.author); 
    if(editor->doc.sprite.pixels) free(editor->doc.sprite.pixels);

    editor->doc = sprite_doc_load(filename);

    //TODO : check if sprite width and height is less than sprite_doc_window width and height
}
void sprite_editor_load_sprite(SpriteEditor * editor,const SpriteDoc * doc)
{
    //TODO : check if sprite width and height is less than sprite_doc_window width and height

    if(editor->doc.name)          free(editor->doc.name);
    if(editor->doc.author)        free(editor->doc.author); 
    if(editor->doc.sprite.pixels) free(editor->doc.sprite.pixels);

    editor->doc = sprite_doc_create_from_sprite(doc->name,doc->author,&doc->sprite);
}
void sprite_editor_render_sprite(SpriteEditor * editor,size_t x,size_t y)
{
    fill_rectangle(editor->sprite_doc_window.x,
                   editor->sprite_doc_window.y,
                   editor->sprite_doc_window.width,
                   editor->sprite_doc_window.height,
                   ATSIN_RGB(0,0,0));
    
    for(size_t posy = 0;posy < editor->doc.sprite.height;++posy)
    {
        for(size_t posx = 0;posx < editor->doc.sprite.width;++posx)
        {
            size_t offset = (posx * editor->doc.sprite.width) + posy;

            drawpixel(editor->sprite_doc_window.x+x+posx,
                      editor->sprite_doc_window.y+y+posy,colors_table[editor->doc.sprite.pixels[offset]]);
        }
    }
}
void sprite_editor_edit_sprite(SpriteEditor * editor,size_t x,size_t y,uint8_t color)
{
    size_t offset = (x * editor->doc.sprite.width) + y;
    editor->doc.sprite.pixels[offset] = color;
}
void sprite_editor_draw_sprite(SpriteEditor * editor,size_t x,size_t y)
{
    fill_rectangle(editor->sprite_doc_window.x,
                   editor->sprite_doc_window.y,
                   editor->sprite_doc_window.width,
                   editor->sprite_doc_window.height,
                   ATSIN_RGB(0,0,0));

    for(size_t posy = 0;posy < editor->doc.sprite.height;++posy)
    {
        for(size_t posx = 0;posx < editor->doc.sprite.width;++posx)
        {
            size_t offset = (posx * editor->doc.sprite.width) + posy;
            fill_rectangle(editor->sprite_doc_window.x+x+posx*editor->doc.sprite.pixelsize,
                               editor->sprite_doc_window.y+y+posy*editor->doc.sprite.pixelsize,
                               editor->doc.sprite.pixelsize-1,
                               editor->doc.sprite.pixelsize-1,
                               colors_table[editor->doc.sprite.pixels[offset]]);
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
                case SDL_KEYDOWN:
                   on_keypressed(editor,g_sdl_event.key.keysym.scancode);
                break;
            }
        }

        if(editor->edit_mode)
        {
            sprite_editor_draw_sprite(editor,0,0);
            if(editor->cursor.selected)
            {
                if(editor->cursor.x < editor->doc.sprite.width &&
                   editor->cursor.y < editor->doc.sprite.height)
                {
                    sprite_editor_edit_sprite(editor,
                                              editor->cursor.x,editor->cursor.y,
                                              editor->cursor.color);
                }
                editor->cursor.selected = 0;
            }
            drawcursor(editor,
                       editor->sprite_doc_window.x,
                       editor->sprite_doc_window.y,
                       editor->cursor.color);

            editor->doc.modified = 1;
        }
        if(editor->doc.auto_save && editor->doc.modified)
        {
            sprite_doc_save(&editor->doc);
            editor->doc.modified = 0;
        }

        if(editor->color_mode)
        {
            const uint32_t color = 15;
            size_t offset = 0;
            for(size_t posy = 0;posy < editor->colors_window.height/editor->cursor.pixelsize;++posy)
            {
                for(size_t posx = 0;posx < editor->colors_window.width/editor->cursor.pixelsize;++posx)
                {
                    if(offset > COLORS_TABLE_SIZE) break;

                    if(editor->cursor.selected && editor->cursor.x == posx && editor->cursor.y == posy)
                       editor->cursor.color = offset;

                    fill_rectangle(editor->colors_window.x+posx*editor->cursor.pixelsize,
                                   editor->colors_window.y+posy*editor->cursor.pixelsize,
                                   editor->cursor.pixelsize-1,
                                   editor->cursor.pixelsize-1,
                                   colors_table[offset]);
                   ++offset;
                }
            }

            drawcursor(editor,editor->colors_window.x,editor->colors_window.y,color);
        }
        SDL_UpdateWindowSurface(g_sdl_window);
    }
}

void sprite_editor_exit(SpriteEditor * editor)
{
    sprite_doc_clearup(&editor->doc);
    free(editor->title);
    free(editor);
    
    SDL_DestroyWindow(g_sdl_window);
}
