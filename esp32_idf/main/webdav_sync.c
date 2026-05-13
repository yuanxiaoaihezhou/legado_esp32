#include "include/webdav_sync.h"

#include <stdio.h>
#include <string.h>

/* URL overhead for "%s/%s/%s.json": 2 slashes + ".json"(5) + '\0' */
#define WEBDAV_URL_OVERHEAD 8

static int build_progress_payload(const book_progress_t *progress, uint8_t *payload, size_t payload_size) {
    if (progress == NULL || payload == NULL || payload_size == 0) return -1;
    int n = snprintf(
        (char *)payload,
        payload_size,
        "{\"bookId\":\"%s\",\"chapterId\":%d,\"pageIndex\":%d,\"timestampMs\":%llu}",
        progress->book_id,
        progress->chapter_id,
        progress->page_index,
        (unsigned long long)progress->timestamp_ms
    );
    if (n < 0 || (size_t)n >= payload_size) return -1;
    return n;
}

void webdav_client_init(webdav_client_t *client, const webdav_config_t *cfg, webdav_transport_fn transport) {
    if (client == NULL || cfg == NULL) return;
    memset(client, 0, sizeof(*client));
    client->cfg = *cfg;
    client->transport = transport;
    if (client->cfg.max_retries < 1) {
        client->cfg.max_retries = 1;
    }
}

bool webdav_upload_progress(webdav_client_t *client, const book_progress_t *progress) {
    if (client == NULL || progress == NULL || client->transport == NULL) return false;

    char url[WEBDAV_MAX_ENDPOINT + WEBDAV_MAX_REMOTE_PATH + WEBDAV_MAX_BOOK_ID + WEBDAV_URL_OVERHEAD] = {0};
    int url_len = snprintf(
        url,
        sizeof(url),
        "%s/%s/%s.json",
        client->cfg.endpoint,
        client->cfg.remote_path,
        progress->book_id
    );
    if (url_len < 0 || (size_t)url_len >= sizeof(url)) {
        return false;
    }

    uint8_t payload[WEBDAV_MAX_PAYLOAD] = {0};
    int payload_len = build_progress_payload(progress, payload, sizeof(payload));
    if (payload_len <= 0) return false;

    for (int i = 0; i < client->cfg.max_retries; i++) {
        int status_code = 0;
        int written = client->transport(
            "PUT",
            url,
            "application/json",
            payload,
            (size_t)payload_len,
            NULL,
            0,
            &status_code
        );
        if (written >= 0 && (status_code == 200 || status_code == 201 || status_code == 204)) {
            return true;
        }
    }
    return false;
}

bool webdav_download_progress(webdav_client_t *client, const char *book_id, book_progress_t *out_progress) {
    if (client == NULL || book_id == NULL || out_progress == NULL || client->transport == NULL) {
        return false;
    }
    char url[WEBDAV_MAX_ENDPOINT + WEBDAV_MAX_REMOTE_PATH + WEBDAV_MAX_BOOK_ID + WEBDAV_URL_OVERHEAD] = {0};
    int url_len = snprintf(
        url,
        sizeof(url),
        "%s/%s/%s.json",
        client->cfg.endpoint,
        client->cfg.remote_path,
        book_id
    );
    if (url_len < 0 || (size_t)url_len >= sizeof(url)) {
        return false;
    }

    uint8_t body[WEBDAV_MAX_PAYLOAD] = {0};
    int status_code = 0;
    int body_size = client->transport(
        "GET",
        url,
        "application/json",
        NULL,
        0,
        body,
        sizeof(body),
        &status_code
    );
    if (body_size <= 0 || status_code != 200) return false;

    /*
     * Current stub limitation:
     * Response JSON is not parsed yet. Returning false avoids reporting
     * a successful sync with invalid zero-filled progress data.
     * TODO: parse body JSON and populate out_progress.
     */
    memset(out_progress, 0, sizeof(*out_progress));
    (void)snprintf(out_progress->book_id, sizeof(out_progress->book_id), "%s", book_id);
    return false;
}
