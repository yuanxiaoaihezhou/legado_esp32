#include "include/content_provider_client.h"

#include <stdio.h>
#include <string.h>

provider_error_t content_provider_fetch_chapter(
    const char *api_base,
    const char *book_id,
    int chapter_id,
    chapter_response_t *out_response
) {
    if (api_base == NULL || book_id == NULL || out_response == NULL || chapter_id < 0) {
        return PROVIDER_ERR_INVALID_ARGUMENT;
    }

    if (strncmp(api_base, "http", 4) != 0) {
        return PROVIDER_ERR_UNSUPPORTED_SOURCE;
    }

    memset(out_response, 0, sizeof(*out_response));
    out_response->ok = true;
    out_response->chapter_id = chapter_id;

    (void)snprintf(
        out_response->title,
        sizeof(out_response->title),
        "Book:%s Chapter:%d",
        book_id,
        chapter_id
    );
    (void)snprintf(
        out_response->content,
        sizeof(out_response->content),
        "This is simplified chapter content from upstream parser service. "
        "book=%s chapter=%d. "
        "In production, ESP32 should pull normalized chapter payload instead of running full source rules.",
        book_id,
        chapter_id
    );

    return PROVIDER_OK;
}

