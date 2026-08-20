# Flame Content SDK

内容分发 + 激励广告一体化 iOS SDK，基于穿山甲内容栈与 Direct GroMore 聚合。

## 版本信息

- **当前版本**：`0.1.0`（准备发布 `0.1.0-beta`）
- **Pod**：`flame_content_sdk_ios`
- **最低支持**：iOS 13.0+
- **语言**：Objective-C
- **内容运行环境**：真机 arm64（内容栈无模拟器切片）

## 快速开始

```ruby
# Podfile
source 'https://cdn.cocoapods.org/'
source '<Flame spec repo>'   # 含 volcengine 内容栈依赖

pod 'flame_content_sdk_ios'
```

```objc
#import <flame_content_sdk_ios/flame_content_sdk_ios.h>

// 1. 初始化（appId/appKey 为 Flame 平台凭证）
[FlameContentSdk initWithAppId:@"YOUR_APP_ID"
                        appKey:@"YOUR_APP_KEY"
                      callback:callback];

// 2. 打开内容聚合页
if (UIViewController *vc = [FlameContentSdk contentAggregatePage]) {
    [self presentViewController:vc animated:YES completion:nil];
}
```

## 目录结构

```
flame_content_sdk_ios/
├── Public/                  # 对外 API
│   ├── FlameContentSdk.h    # 初始化 / Reward
│   ├── FlameContentEntry.h  # 内容三入口
│   └── FlameContentAuxiliary.h  # 解锁策略 / 搜索 / 历史 / 收藏 / 登录
├── core/                    # 初始化/配置/状态
├── ads/                     # GroMore 激励广告
├── content/                 # CSJ 内容 Provider / Runtime
├── unlock/                  # Custom Unlock Bridge
├── security/                # 签名 / AES-GCM
├── network/                 # /sdk/init
├── Example/                 # 演示工程
└── Tests/                   # 138 测试
```

## 核心能力

| 能力 | API |
|---|---|
| 初始化 | `initWithAppId:appKey:callback:` |
| 内容聚合页 | `contentAggregatePage` |
| 短剧详情页 | `contentPlayletPageForSkitId:episode:` |
| 滑滑流 | `contentFeedPage` |
| 激励广告 | `createRewardAdWithViewController:placementId:listener:` |
| 解锁策略 | `setContentUnlockConfigWithFreeEpisodeCount:unlockEpisodeCount:` |
| 搜索 | `searchPlayletsWithKeyword:success:failure:` |
| 历史 | `requestContinueWatchingWithSuccess:failure:` |
| 收藏 | `favoritePlayletWithPlayletId:...` / `requestFavoritePlaylets...` |
| 登录 | `contentLoginWithParamsString:completion:` |

## 版本快照

- `docs/release/versions/0.1.0/RELEASE_SNAPSHOT_0.1.0-beta.md`

## 文档

- `docs/release/`：接入文档入口（getting-started/ + guides/ + api/ + migration/ + versions/）
