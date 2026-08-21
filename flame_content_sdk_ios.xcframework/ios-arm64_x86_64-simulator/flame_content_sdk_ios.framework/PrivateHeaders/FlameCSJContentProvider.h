#import <Foundation/Foundation.h>
#import "../provider/FlameContentProvider.h"

NS_ASSUME_NONNULL_BEGIN

/// CSJ/Pangrowth 内容能力 Provider。
///
/// 编译形态：
/// - 真机 + FLAME_CONTENT_DEVICE=1（Content subspec / FLAME_CONTENT=1 Podfile）：真实 DJX 实现；
/// - 模拟器或未启用 Content 依赖：Unavailable stub（可预测错误 14120，不 crash、无三方符号引用）。
///
/// 配置文件解析顺序（宿主无感知，见 docs/context/接入内容SDK/02）：
///   1) Flame framework bundle 内 sdk_setting_file.json（Flame 每客户定制包携带）
///   2) 宿主 main bundle 内 sdk_setting_file.json（兼容官方拖入式接入）
@interface FlameCSJContentProvider : NSObject <FlameContentProvider>

@end

NS_ASSUME_NONNULL_END
