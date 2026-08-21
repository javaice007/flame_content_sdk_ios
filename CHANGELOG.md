# CHANGELOG

> Flame Content SDK 版本变更记录。格式参考老 flame_ios_sdk `docs/releases/` 规范。

## [0.1.0] - 未发布（准备 0.1.0-beta）

### 新增

- **初始化链路**：`/sdk/init`（MD5 签名 + AES-GCM edata 解密）+ GroMore init + Content init/start
- **内容能力**（PangrowthX 2.9.0.6 + TTSDKFramework Player-SR 1.42.3.4-premium）
  - 聚合页 `contentAggregatePage`
  - 详情页 `contentPlayletPageForSkitId:episode:`
  - 滑滑流 `contentFeedPage`
- **Custom Unlock Bridge**：scenePIds.content_unlock → Flame Placement → GroMore Reward →
  曝光/cpm/激励验证 → Content 解锁流程（onADWillShow / onADRewardDidVerified / unlockFlowEnd）
- **激励广告**：Direct GroMore Reward 全生命周期（load/show/exposure/verify/close，once-only，主线程）
- **解锁策略**：`setContentUnlockConfigWithFreeEpisodeCount:unlockEpisodeCount:`（默认 10/5）
- **辅助能力**：搜索 / 历史（继续观看）/ 收藏闭环 / 按 ID 查询 / 清空历史
- **内容登录**：`contentLoginWithParamsString:completion:`（Host Backend 签名，server key 不进客户端）
- **单 Pod 集成**：`pod 'flame_content_sdk_ios'` 依赖自动闭合
- **发布名称统一（A6.1）**：工程/Pod/XCFramework 由 `flame_ios_content_sdk` 更名
  `flame_content_sdk_ios`；Public API（`FlameContentSdk`/`FlameContentEntry`/`FlameContentAuxiliary`）
  与 import 路径同步更新，零 breaking
- **XCFramework**：双切片（ios-arm64 + simulator）+ 三方符号污染门禁

### 修复

- 黑屏根因：旧栈 PangrowthTTVideoEngine-dynamic 1.0.0.0 drawableSize={0,0} → 迁移新栈
- Feed 播放：attach 模式覆盖（Common→Specific）阻断 SDK 广告填充 → 修复
- 详情页 delegate reattach：滑滑流→新详情页双通道重挂

### 已知限制

- 内容栈 device-only（模拟器返回 14120）
- 辅助列表 one-shot（最多 50 条，无 load-more）
- Content Login Live 待 Host Backend 签名
