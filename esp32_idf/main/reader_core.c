#include "include/reader_core.h"

#include <string.h>

void reader_init(reader_state_t *state) {
    if (state == NULL) return;
    memset(state, 0, sizeof(*state));
}

size_t reader_paginate_chapter(
    int chapter_id,
    const char *chapter_text,
    size_t bytes_per_page,
    reader_page_t *out_pages,
    size_t max_pages
) {
    if (chapter_text == NULL || out_pages == NULL || bytes_per_page == 0 || max_pages == 0) {
        return 0;
    }
    if (bytes_per_page > READER_MAX_PAGE_TEXT - 1) {
        bytes_per_page = READER_MAX_PAGE_TEXT - 1;
    }

    size_t text_len = strlen(chapter_text);
    if (text_len == 0) {
        out_pages[0].chapter_id = chapter_id;
        out_pages[0].page_index = 0;
        out_pages[0].is_last_page = true;
        out_pages[0].text[0] = '\0';
        return 1;
    }

    size_t page_index = 0;
    size_t offset = 0;
    while (offset < text_len && (size_t)page_index < max_pages) {
        size_t remaining = text_len - offset;
        size_t copy_len = remaining > bytes_per_page ? bytes_per_page : remaining;

        out_pages[page_index].chapter_id = chapter_id;
        out_pages[page_index].page_index = (int)page_index;
        out_pages[page_index].is_last_page = (offset + copy_len >= text_len);
        memcpy(out_pages[page_index].text, chapter_text + offset, copy_len);
        out_pages[page_index].text[copy_len] = '\0';

        offset += copy_len;
        page_index++;
    }

    if (offset < text_len && page_index > 0) {
        out_pages[page_index - 1].is_last_page = true;
    }
    return page_index;
}

bool reader_get_page(
    const reader_page_t *pages,
    size_t total_pages,
    int page_index,
    reader_page_t *out_page
) {
    if (pages == NULL || out_page == NULL || page_index < 0) return false;
    if ((size_t)page_index >= total_pages) return false;
    *out_page = pages[page_index];
    return true;
}
