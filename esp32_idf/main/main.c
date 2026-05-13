#include "include/app_orchestrator.h"

#include <stdio.h>
#include <string.h>

static int mock_webdav_transport(
    const char *method,
    const char *url,
    const char *content_type,
    const uint8_t *body,
    size_t body_size,
    uint8_t *out_body,
    size_t out_body_capacity,
    int *out_status_code
) {
    (void)out_body;
    (void)out_body_capacity;
    if (out_status_code == NULL || method == NULL || url == NULL) return -1;

    if (strcmp(method, "PUT") == 0) {
        printf("WebDAV PUT %s (%s): %.*s\n", url, content_type, (int)body_size, (const char *)body);
        *out_status_code = 201;
        return (int)body_size;
    }

    if (strcmp(method, "GET") == 0) {
        *out_status_code = 200;
        return 0;
    }

    *out_status_code = 405;
    return -1;
}

void app_main(void) {
    webdav_config_t cfg = {0};
    (void)snprintf(cfg.endpoint, sizeof(cfg.endpoint), "%s", "https://example.com/dav");
    (void)snprintf(cfg.remote_path, sizeof(cfg.remote_path), "%s", "bookProgress");
    (void)snprintf(cfg.username, sizeof(cfg.username), "%s", "user");
    (void)snprintf(cfg.password, sizeof(cfg.password), "%s", "pass");
    cfg.max_retries = 3;

    app_runtime_t runtime = {
        .content_api_base = "https://content-service.local/api",
        .book_id = "demo-book",
        .chapter_id = 1,
        .bytes_per_page = 180
    };
    webdav_client_init(&runtime.webdav, &cfg, mock_webdav_transport);

    if (!app_init(&runtime)) {
        printf("app_init failed\n");
        return;
    }
    if (!app_cycle(&runtime)) {
        printf("app_cycle failed\n");
        return;
    }
    printf("app_cycle done\n");
}
