#ifndef READER_CORE_H
#define READER_CORE_H

#include <stdbool.h>
#include <stddef.h>

#define READER_MAX_PAGE_TEXT 256
#define READER_MAX_PAGES 64

typedef struct {
    int chapter_id;
    int page_index;
    bool is_last_page;
    char text[READER_MAX_PAGE_TEXT];
} reader_page_t;

typedef struct {
    int chapter_id;
    int total_pages;
    int current_page;
    size_t last_offset_bytes;
} reader_state_t;

void reader_init(reader_state_t *state);

size_t reader_paginate_chapter(
    int chapter_id,
    const char *chapter_text,
    size_t bytes_per_page,
    reader_page_t *out_pages,
    size_t max_pages
);

bool reader_get_page(
    const reader_page_t *pages,
    size_t total_pages,
    int page_index,
    reader_page_t *out_page
);

#endif

