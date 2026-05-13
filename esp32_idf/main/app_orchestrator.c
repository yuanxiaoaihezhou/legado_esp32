#include "include/app_orchestrator.h"
#include "include/content_provider_client.h"

#include <stdio.h>
#include <string.h>

bool app_init(app_runtime_t *runtime) {
    if (runtime == NULL || runtime->content_api_base == NULL || runtime->book_id == NULL) {
        return false;
    }
    if (runtime->bytes_per_page == 0) {
        runtime->bytes_per_page = 180;
    }
    reader_init(&runtime->reader);
    runtime->total_pages = 0;
    return true;
}

bool app_cycle(app_runtime_t *runtime) {
    if (runtime == NULL) return false;

    chapter_response_t chapter = {0};
    provider_error_t err = content_provider_fetch_chapter(
        runtime->content_api_base,
        runtime->book_id,
        runtime->chapter_id,
        &chapter
    );
    if (err != PROVIDER_OK || !chapter.ok) {
        return false;
    }

    runtime->total_pages = reader_paginate_chapter(
        chapter.chapter_id,
        chapter.content,
        runtime->bytes_per_page,
        runtime->pages,
        READER_MAX_PAGES
    );
    if (runtime->total_pages == 0) {
        return false;
    }

    runtime->reader.chapter_id = chapter.chapter_id;
    runtime->reader.total_pages = (int)runtime->total_pages;
    runtime->reader.current_page = 0;
    runtime->reader.last_offset_bytes = strlen(runtime->pages[0].text);

    /* 这里可替换为真实 e-Paper 渲染输出 */
    printf("Render chapter=%d page=%d/%d\n%s\n",
           runtime->reader.chapter_id,
           runtime->reader.current_page + 1,
           runtime->reader.total_pages,
           runtime->pages[0].text);

    book_progress_t progress = {0};
    (void)snprintf(progress.book_id, sizeof(progress.book_id), "%s", runtime->book_id);
    progress.chapter_id = runtime->reader.chapter_id;
    progress.page_index = runtime->reader.current_page;
    progress.timestamp_ms = 0;
    return webdav_upload_progress(&runtime->webdav, &progress);
}

