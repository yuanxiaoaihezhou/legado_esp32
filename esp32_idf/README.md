# ESP32-S3 精简迁移实现（ESP-IDF C）

本目录提供一个可编译的**精简版迁移骨架**，用于落地以下边界：

- ✅ ESP32 端：阅读分页/渲染抽象、WebDAV 进度同步、内容拉取编排
- ✅ 上游内容：通过“已解析章节接口”获取（不在 ESP32 端复刻完整书源引擎）
- ❌ 不包含 Legado Android 完整等价能力（复杂规则解析、Rhino JS、完整 UI 体系）

## 目录结构

- `main/include/reader_core.h`：文本分页核心接口（内置 `READER_TARGET_WIDTH/HEIGHT=200` 目标参数）
- `main/include/content_provider_client.h`：上游“已解析章节”客户端接口（简化版）
- `main/include/webdav_sync.h`：WebDAV 进度同步接口与重试策略
- `main/include/app_orchestrator.h`：设备侧编排入口
- `main/*.c`：接口最小实现与可运行示例

## 设计要点

1. **阅读能力精简**：仅保留“章节文本 -> 分页 -> 当前页”链路，便于接入 e-Paper 驱动。
2. **书源能力下沉**：由手机/服务端输出标准化章节内容，ESP32 只消费结果。
3. **同步优先落地**：WebDAV 作为独立模块，后续可替换为真实 HTTP/TLS 传输层。

## 配套 App 对接建议

- 建议使用 App Web API：`/getEsp32Chapter?url={bookUrl}&index={chapterIndex}`。
- 返回字段：`bookId`、`chapterIndex`、`title`、`content`，可直接映射到设备侧章节拉取模型。
- 进度同步 JSON 字段与设备侧一致：`bookId`、`chapterId`、`pageIndex`、`timestampMs`。

## 后续接入

- 用真实 e-Paper 驱动替换 `app_orchestrator.c` 中的页面输出逻辑。
- 用 ESP-IDF `esp_http_client` 实现 `webdav_transport_fn`。
- 用真实 API 替换 `content_provider_fetch_chapter()` 中的示例内容。
