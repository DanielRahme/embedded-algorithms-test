#ifndef _CACHE_H
#define _CACHE_H

void cache_enable();
void cache_disable();
int cache_hit();
int cache_miss();
void cache_hit_miss_reset();

#endif