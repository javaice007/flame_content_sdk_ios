#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/// CSJ 内容页运行时包装（A4B；**内部能力，非 Public API**）。
///
/// 设备 + FLAME_CONTENT_DEVICE：包装三类真实内容页创建入口，创建即挂接 Custom Unlock
/// （Specific 模式 + interfaceDelegate，经 FlamePlayletAttachCenter）：
///   ① 聚合页（DJXPlayletAggregatePageViewController，config.playletConfig）
///   ② 短剧详情页（DJXPlayletManager playletViewControllerWithParams:）
///   ③ 滑滑流（DJXDrawVideoViewController，VCConfig.playletConfig + playerDelegate 重挂器）
/// 滑滑流 → 新详情页实例的 delegate 重挂：官方 28261 要求；经 playerDelegate
/// （drawVideoStartPlay:）与 interfaceDelegate（clickEnterView:）双通道触发纯核心重挂。
///
/// 模拟器 / 未启用：零三方符号；返回 nil（Content 14120 不可用语义）。
@interface FlameCSJContentRuntime : NSObject

+ (instancetype)sharedRuntime;

/// A4D 测试调参：免费集数 / 一次激励解锁集数覆盖（<=0 时使用内部默认；仅供 Example/harness
/// 使用，非客户 Public API；生产值后续迁后端下发）。免费集数受官方强规则约束（≤20 或前 20% 取大）。
+ (void)flame_setFreeEpisodesOverride:(NSInteger)count;
+ (void)flame_setUnlockEpisodesPerAdOverride:(NSInteger)count;

/// 内容就绪（Provider ready + SDK 初始化完成）才可创建页面；否则返回 nil。
- (nullable UIViewController *)aggregatePageViewController;

- (nullable UIViewController *)playletDetailViewControllerForSkitId:(NSInteger)skitId
                                                            episode:(NSInteger)episode;

- (nullable UIViewController *)drawFeedViewController;

/// Phase 1 的独立嵌入式 Feed Container 创建路径；不影响旧 drawFeedViewController 行为。
- (nullable UIViewController *)feedContainerViewController;

@end

NS_ASSUME_NONNULL_END
