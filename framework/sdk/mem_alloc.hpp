#pragma once

class c_mem_alloc {
public:
	virtual ~c_mem_alloc( ) = 0;
	virtual void* alloc( size_t sz ) = 0;
	virtual void* realloc( void* mem, size_t sz ) = 0;
	virtual void free( void* mem ) = 0;
};