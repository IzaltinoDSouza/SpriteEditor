#include "sprite.h"
#include "include/atsin/string_utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool atsin_sprite_header_check(uint8_t * header_signature)
{
    return strcmp(header_signature,"[AtsinSprite]") == 0;
}
Sprite atsin_sprite_load(char * filename)
{
    Sprite sprite = {.width=0,.height=0,.pixelsize=0,.pixels=NULL};
    
    FILE * fp = fopen(filename,"r");
    if(fp)
    {
        uint8_t header_signature[14];
        fscanf(fp,"%s",header_signature);
        if(atsin_sprite_header_check(header_signature))
        {
            fscanf(fp,"%d",&sprite.width);  
            fscanf(fp,"%d",&sprite.height);
            fscanf(fp,"%hhd\n",&sprite.pixelsize);
            
            /*
            printf("%d ",sprite.width);
            printf("%d ",sprite.height);
            printf("%hhd\n",sprite.pixelsize);
            */        
            
            sprite.pixels = malloc(sprite.width*sprite.height);
            
            for(size_t y = 0;y < sprite.height;++y)
            {
                for(size_t x = 0;x < sprite.width;++x)
                {
                    char c;
                    fscanf(fp,"%c",&c); 
                    //printf("%c",c);
                                    
                    size_t offset = (x * sprite.width) + y;
                    sprite.pixels[offset] = c - '0';
                }
                
                //consome new line
                char separator;
                fscanf(fp,"%c",&separator);
                //printf("\n");
                
                //TODO error : {filename} is not a valid sprite
            }
            
        }else
        {
            //TODO error : {filename} is not a valid sprite
        }
    }else
    {
        //TODO : {filename} cannot be opened
    }
    return sprite;
}
void atsin_sprite_save(char * filename,const Sprite * sprite)
{
    FILE * fp = fopen(filename,"w");
    if(fp)
    {
    
        fprintf(fp,"[AtsinSprite]\n");
        fprintf(fp,"%d\n",sprite->width);  
        fprintf(fp,"%d\n",sprite->height);
        fprintf(fp,"%hhd\n",sprite->pixelsize);
        
        for(size_t y = 0;y < sprite->height;++y)
        {
            for(size_t x = 0;x < sprite->width;++x)
            {
                size_t offset = (x * sprite->width) + y;
                fprintf(fp,"%c",sprite->pixels[offset] + '0');
            }
            fprintf(fp,"\n");
        }
    }else
    {
        //TODO : {filename} cannot be opened
    }
}
Sprite sprite_load(char * filename)
{    
    if(end_with(filename,".sprite"))
    {
       return atsin_sprite_load(filename);
    }
    else if(end_with(filename,".qoi"))
    {
        //TODO qoi_sprite_load(filename);
    }else{
        //TODO error : file format is not supported
    }
}
void sprite_save(char * filename,const Sprite * sprite)
{
    if(end_with(filename,".sprite"))
    {
       return atsin_sprite_save(filename,sprite);
    }
    else if(end_with(filename,".qoi"))
    {
        //TODO qoi_sprite_save(filename);
    }else{
        //TODO error : file format is not supported
    }
}
