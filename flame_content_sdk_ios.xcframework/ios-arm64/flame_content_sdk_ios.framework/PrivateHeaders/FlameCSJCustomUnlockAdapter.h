#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class DJXPlayletConfig;

/// CSJ Content 自定义解锁 DJX 胶水（A4A）。
/// 设备 + FLAME_CONTENT_DEVICE：实现 `DJXPlayletInterfaceProtocol`（Specific 模式），
///   将 DJX 解锁回调翻译为 `FlameCustomUnlockBridge` 纯核心调用，再按官方契约构造回传。
/// 模拟器/未启用：零三方符号；attach 为 no-op（Content 本身 14120 不可用）。
///
/// 注意：DJXPlayletConfig.interfaceDelegate 为 weak（A1 05），本适配器为单例强持有，
/// 由内部内容页包装（A4B 播放入口）在创建页面时调用 attachToPlayletConfig: 注入。
@interface FlameCSJCustomUnlockAdapter : NSObject

+ (instancetype)sharedAdapter;

/// 注入 Specific 模式 + 本适配器（内部 API；仅内容页包装使用，Host 无感知）。
- (void)attachToPlayletConfig:(nullable DJXPlayletConfig *)config;

/// attach 时捕获的 unlockEpisodesCountUsingAD（echo 给 unlockFlowStart；模拟器形态为 0）。
@property (nonatomic, assign, readonly) NSInteger capturedUnlockEpisodeCount;

@end

NS_ASSUME_NONNULL_END
