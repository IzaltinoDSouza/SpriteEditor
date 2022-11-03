#include "sprite_doc.h"

#include <stdlib.h>
#include <string.h>

SpriteDoc sprite_doc_create(char * name,char * author,size_t width,size_t height,uint8_t pixelsize)
{
    SpriteDoc doc;
    
    //TODO: check if title is NULL
    doc.name = malloc(strlen(name));
    strcpy(doc.name,name);
    
    //TODO: check if author is NULL
    doc.author = malloc(strlen(author));
    strcpy(doc.author,author);
        
    doc.version   = SPRITE_DOC_01_VERSION;
    doc.auto_save = 0;
    doc.modified  = 0;
    
    //sprite set
    doc.sprite.width     = width;
    doc.sprite.height    = height;
    doc.sprite.pixelsize = pixelsize;
    
    //sprite pixels alloc and set
    size_t size = doc.sprite.width*doc.sprite.height;
    doc.sprite.pixels = calloc(size,sizeof(uint8_t));
    
    //TODO : check allocation fail
    
    return doc;
}

SpriteDoc sprite_doc_create_from_sprite(char * name,char * author,const Sprite * sprite)
{
    SpriteDoc doc;
    
    //TODO: check if title is NULL
    doc.name = malloc(strlen(name));
    strcpy(doc.name,name);
    
    //TODO: check if author is NULL
    doc.author = malloc(strlen(author));
    strcpy(doc.author,author);
        
    doc.version   = SPRITE_DOC_01_VERSION;
    doc.auto_save = 0;
    doc.modified  = 0;
    
    //sprite set
    doc.sprite.width     = sprite->width;
    doc.sprite.height    = sprite->height;
    doc.sprite.pixelsize = sprite->pixelsize;
    
    //sprite pixels alloc
    size_t size = doc.sprite.width*doc.sprite.height;
    doc.sprite.pixels = malloc(size);
    //TODO : check allocation fail
    
    //sprite pixels copy
    //TODO : check sprite->pixels is null
    memcpy(doc.sprite.pixels,sprite->pixels,size);
    //TODO : check copy fail    
    
    return doc;
}

SpriteDoc sprite_doc_load(const char * filename)
{
    //TODO : it need to be implemented
}

void sprite_doc_save(const SpriteDoc * doc)
{
    //TODO : it need to be implemented
}

void sprite_doc_clearup(SpriteDoc * doc)
{
    free(doc->name);
    doc->name = NULL;
    free(doc->author);
    doc->author = NULL;
    free(doc->sprite.pixels);
    doc->sprite.pixels = NULL;
}
