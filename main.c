#include <SDL2/SDL.h>
#include <stdbool.h>

#include "sprite.h"
#include "editor.h"
#include "sprite_doc.h"

int main()
{
    Sprite test = sprite_load("demo.sprite");

    SpriteDoc doc = sprite_doc_create_from_sprite("enemy","atsindev",&test);

    SpriteEditor * editor = sprite_editor_init("Sprite Editor",512,512,16);

    sprite_editor_load_sprite(editor,&doc);

    sprite_editor_draw_sprite(editor,0,0);
    //sprite_editor_render_sprite(editor,0,0);

    sprite_editor_event_loop(editor);

    sprite_editor_exit(editor);

    sprite_doc_clearup(&doc);
}
