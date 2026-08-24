#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "FlameNavigationCoordinator.h"

@class FlameContentPlaylet;

NS_ASSUME_NONNULL_BEGIN

/// 可见性冻结参数（39 号 §1；契约测试直接断言这些常量）。
extern CGFloat const FlameStreamDominantVisibleRatio;       ///< 0.5（item 自身可见面积占比）
extern NSTimeInterval const FlameStreamActiveStableInterval; ///< 0.3（active 切换稳定延迟）
extern NSTimeInterval const FlameStreamExposureDuration;     ///< 0.8（曝光持续阈值）
extern NSTimeInterval const FlameStreamExposureDedupInterval;///< 3.0（重复曝光最小间隔）

/// 一次 Stream 实例的上下文（36 号 §1.1；仅诊断，无权益/播放字段）。
@interface FlameFeedStreamSession : NSObject

@property (nonatomic, copy, readonly) NSString *streamSessionId;   // fs-<单调序列>
@property (nonatomic, copy, readonly, nullable) NSString *ownerComponentId;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithStreamSessionId:(NSString *)streamSessionId
                                 ownerComponentId:(nullable NSString *)ownerComponentId;
/// 内部工厂：自动生成 `fs-<单调序列>` 会话 ID。
+ (nullable instancetype)flame_sessionWithOwnerComponentId:(nullable NSString *)ownerComponentId;

@end

/// 唯一 active item 管理（36 号 §1.2；300ms 稳定 + 原子替换，多 active 断言拒绝）。
typedef void (^FlameStreamActiveItemChangeBlock)(FlameContentPlaylet *_Nullable item);

@interface FlameActiveItemManager : NSObject

@property (nonatomic, strong, readonly, nullable) FlameContentPlaylet *activeItem;

- (instancetype)initWithOnActiveItemChange:(FlameStreamActiveItemChangeBlock)onChange;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// 主导候选变更输入（来自 VisibilityResolver；稳定 300ms 后原子提交 active）。
- (void)noteDominantCandidate:(nullable FlameContentPlaylet *)item;

/// 取消未决切换（Released / 后台；active 保持不变）。
- (void)cancelPendingSwitch;

@end

/// 可见性判定与曝光（36 号 §1.3；参数见上方常量，后台暂停计时）。
typedef void (^FlameStreamExposureBlock)(FlameContentPlaylet *item);

@interface FlameVisibilityResolver : NSObject

- (instancetype)initWithActiveItemManager:(FlameActiveItemManager *)manager
                              onExposure:(FlameStreamExposureBlock)onExposure;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// UI 滚动上报（ratio < 主导阈值视为无主导）。
- (void)noteDominantCandidate:(nullable FlameContentPlaylet *)item ratio:(CGFloat)ratio;

/// 后台：取消进行中的曝光窗口（本期主导期结束，不虚计）。
- (void)noteApplicationBackgrounded;

/// 前台：以最近上报的候选重起曝光窗口。
- (void)noteApplicationForegrounded;

@end

/// 播放交接（36 号 §1.4；只发起请求，不创建/不销毁 Player Session）。
@interface FlamePlaybackHandoff : NSObject

- (instancetype)initWithComponentId:(NSString *)componentId
                navigationCoordinator:(FlameNavigationCoordinator *)coordinator;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// item 点击 → 既有 feed-stream→player 显式路由 → Player Runtime 全屏会话。
/// 失败（含路由 fail closed）经 onFailure 回调；播放开始事件由路由 handler 在
/// Player 会话建立成功时通知组件发出（coordinator 注册 handler 不回调 caller completion）。
- (void)requestPlaybackForItem:(FlameContentPlaylet *)item
                      presenter:(nullable UIViewController *)presenter
                       onFailure:(void (^)(NSString *_Nullable code, NSString *_Nullable desc))onFailure;

@end

NS_ASSUME_NONNULL_END
