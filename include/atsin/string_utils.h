#pragma once

#include <stdbool.h>
#include <string.h>

static bool start_with(char * str,char * start)
{
	const size_t len = strlen(start);
	return strncmp(str,start,len) == 0;
}
static bool end_with(char * str,char * end)
{
	const size_t str_len = strlen(str);
	const size_t end_len = strlen(end);
	
	if(end_len > str_len) return false;
	
	char * begin = str + (str_len-end_len);
	return strcmp(begin,end) == 0;
}
