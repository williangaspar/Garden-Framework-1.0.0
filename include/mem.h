#ifndef  __MEM_H__
#define  __MEM_H__

#include <stddef.h>
#include <attrib.h>

void *mem_alloc(size_t);
void mem_free(void *);

#endif  /*MEM_H*/
