#ifndef WEBDAV_SYNC_H
#define WEBDAV_SYNC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define WEBDAV_MAX_ENDPOINT 128
#define WEBDAV_MAX_USERNAME 64
#define WEBDAV_MAX_PASSWORD 64
#define WEBDAV_MAX_REMOTE_PATH 128
#define WEBDAV_MAX_BOOK_ID 64
#define WEBDAV_MAX_PAYLOAD 256

typedef struct {
    char endpoint[WEBDAV_MAX_ENDPOINT];
    char username[WEBDAV_MAX_USERNAME];
    char password[WEBDAV_MAX_PASSWORD];
    char remote_path[WEBDAV_MAX_REMOTE_PATH];
    int max_retries;
} webdav_config_t;

typedef struct {
    char book_id[WEBDAV_MAX_BOOK_ID];
    int chapter_id;
    int page_index;
    uint64_t timestamp_ms;
} book_progress_t;

typedef int (*webdav_transport_fn)(
    const char *method,
    const char *url,
    const char *content_type,
    const uint8_t *body,
    size_t body_size,
    uint8_t *out_body,
    size_t out_body_capacity,
    int *out_status_code
);

typedef struct {
    webdav_config_t cfg;
    webdav_transport_fn transport;
} webdav_client_t;

void webdav_client_init(webdav_client_t *client, const webdav_config_t *cfg, webdav_transport_fn transport);

bool webdav_upload_progress(webdav_client_t *client, const book_progress_t *progress);
bool webdav_download_progress(webdav_client_t *client, const char *book_id, book_progress_t *out_progress);

#endif

