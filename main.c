#include <SDL2/SDL.h>
#include <stdbool.h>

#include "sprite.h"
#include "editor.h"
#include "sprite_doc.h"

int main()
{
    SpriteEditor * editor = sprite_editor_init("Sprite Editor",512,512,16);

    sprite_editor_load_sprite_from_disk(editor,"demo.sprite");

    sprite_editor_draw_sprite(editor,0,0);
    //sprite_editor_render_sprite(editor,0,0);

    sprite_editor_event_loop(editor);

    sprite_editor_exit(editor);
}
