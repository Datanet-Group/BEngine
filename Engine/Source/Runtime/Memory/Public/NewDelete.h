#pragma once
#include "BEngineMemoryAPI.h"
#include "MemoryTags.h"
#include <new>
#include <cstddef>

// Global new/delete are defined in NewDelete.cpp if BENGINE_DEFINE_GLOBAL_NEWDELETE=1
BENGINE_MEMORY_API void* BAlloc(std::size_t size, std::size_t align, EMemoryTag tag);
BENGINE_MEMORY_API void  BFree (void* p);

// Helpers for tagged allocations
#define BNEW_TAG(type, tag, ...)   new (BAlloc(sizeof(type), alignof(type), (tag))) type(__VA_ARGS__)
#define BDELETE(ptr)               do { if(ptr) { (ptr)->~std::remove_pointer_t<decltype(ptr)>(); BFree(ptr); } } while(0)
