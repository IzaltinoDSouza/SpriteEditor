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
    SpriteDoc doc;
    //TODO: check if filename is NULL
    doc.name = malloc(strlen(filename));
    strcpy(doc.name,filename);
    
    //TODO: get author
    doc.author = NULL;
        
    doc.version   = SPRITE_DOC_01_VERSION;
    doc.auto_save = 0;
    doc.modified  = 0;
    
    //TODO check load fail
    doc.sprite = sprite_load((char*)filename);
    
    return doc;
}

void sprite_doc_save(const SpriteDoc * doc)
{
    static const char sprite_extension[] = ".sprite\0";
    const size_t doc_name_size = strlen(doc->name);
    
    //TODO : check allocation fail 
    char * filename = malloc(doc_name_size + 8);    
    
    //copy
    strcpy(filename,doc->name);
    strcpy(filename + doc_name_size,sprite_extension);
    
    //TODO check save fail
    sprite_save(filename,&doc->sprite);
    free(filename);
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
