# Flame Content SDK

内容分发 + 激励广告一体化 iOS SDK，基于穿山甲内容栈与 Direct GroMore 聚合。

> 本仓库为 **Flame Content SDK 二进制分发仓库**（dist repo，对齐 flame_sdk_ios 发布模型）：
> 仅包含 xcframework 与发布元数据；SDK 源码在独立源码仓库开发。版本通过 **Git Tag** 管理。

## 当前版本

`0.1.0`（发布通道 `0.1.0-beta`）

## 发布形态

| 组成 | 说明 |
|---|---|
| `flame_content_sdk_ios.xcframework` | 唯一二进制包（ios-arm64 真机含内容实现；simulator 切片为桩，内容 device-only） |
| `README.md` / `CHANGELOG.md` / `LICENSE` / `VERSION` / `SHA256SUMS` | 发布元数据与完整性校验 |

## 客户接入方式

```ruby
source 'https://github.com/javaice007/flame-specs.git'
source 'https://cdn.cocoapods.org/'
source 'https://github.com/volcengine/volcengine-specs.git'   # 内容栈三方源

target 'YourApp' do
  use_frameworks! :linkage => :static
  pod 'flame_content_sdk_ios', '0.1.0'
end
```

接入代码：

```objc
#import <flame_content_sdk_ios/flame_content_sdk_ios.h>
```

```swift
import flame_content_sdk_ios
```

## 版本历史

- `0.1.0`（通道 0.1.0-beta，2026-08-21）：首个二进制发布——初始化链路、内容三入口、
  激励解锁、辅助能力（搜索/历史/收藏）、内容登录；详见 `CHANGELOG.md`。

## 完整性校验

```bash
shasum -a 256 -c SHA256SUMS --ignore-missing
```

（覆盖 README/CHANGELOG/LICENSE/VERSION 与两切片主二进制。）
