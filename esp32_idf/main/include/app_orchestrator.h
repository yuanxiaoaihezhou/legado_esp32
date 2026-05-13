#ifndef APP_ORCHESTRATOR_H
#define APP_ORCHESTRATOR_H

#include <stdbool.h>
#include "reader_core.h"
#include "webdav_sync.h"

typedef struct {
    const char *content_api_base;
    const char *book_id;
    int chapter_id;
    size_t bytes_per_page;
    webdav_client_t webdav;
    reader_state_t reader;
    reader_page_t pages[READER_MAX_PAGES];
    size_t total_pages;
} app_runtime_t;

bool app_init(app_runtime_t *runtime);
bool app_cycle(app_runtime_t *runtime);

#endif

