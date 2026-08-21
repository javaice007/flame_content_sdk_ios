#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// sdk_setting_file.json 解析器（纯 Foundation，无三方符号；设备/模拟器双形态共用，可单测）。
///
/// 查找顺序（A3.1 冻结 B2 客户分发，docs/context/接入内容SDK/10_A3_1_CUSTOMER_DISTRIBUTION.md）：
///   1. 宿主内 FlameCustomerConfig.bundle —— Flame 按客户定制包携带（正式分发通道）；
///   2. Flame framework bundle 自身（随包携带，技术上可行通道）；
///   3. 宿主 main bundle 根（官方拖入式兼容）。
@interface FlameContentConfigResolver : NSObject

/// 客户定制 resource bundle 名（与 Scripts/package_customer_content_sdk.sh 的产物契约，勿改）。
@property (class, nonatomic, copy, readonly) NSString *customerBundleName;

/// 按冻结顺序解析当前进程内的配置文件路径；未找到返回 nil（结果 dispatch_once 缓存）。
+ (nullable NSString *)resolveConfigPathForClass:(Class)ownerClass;

@end

NS_ASSUME_NONNULL_END
