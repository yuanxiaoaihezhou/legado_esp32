#ifndef CONTENT_PROVIDER_CLIENT_H
#define CONTENT_PROVIDER_CLIENT_H

#include <stdbool.h>
#include <stddef.h>

#define PROVIDER_MAX_TITLE 64
#define PROVIDER_MAX_CONTENT 2048

typedef enum {
    PROVIDER_OK = 0,
    PROVIDER_ERR_INVALID_ARGUMENT,
    PROVIDER_ERR_UNSUPPORTED_SOURCE,
    PROVIDER_ERR_NETWORK,
    PROVIDER_ERR_PARSE
} provider_error_t;

typedef struct {
    bool ok;
    int chapter_id;
    char title[PROVIDER_MAX_TITLE];
    char content[PROVIDER_MAX_CONTENT];
} chapter_response_t;

provider_error_t content_provider_fetch_chapter(
    const char *api_base,
    const char *book_id,
    int chapter_id,
    chapter_response_t *out_response
);

#endif
