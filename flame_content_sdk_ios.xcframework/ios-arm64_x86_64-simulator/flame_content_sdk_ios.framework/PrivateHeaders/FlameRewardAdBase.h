#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameRewardAd.h>
#import "FlameEcpmInfo.h"

NS_ASSUME_NONNULL_BEGIN

/// Reward 广告对象内部状态机（对 Host 不可见）。
typedef NS_ENUM(NSInteger, FlameRewardAdInternalState) {
    FlameRewardAdInternalStateCreated = 0,
    FlameRewardAdInternalStateLoading,
    FlameRewardAdInternalStateLoaded,
    FlameRewardAdInternalStateShowing,
    FlameRewardAdInternalStateRewarded,
    FlameRewardAdInternalStateSkipped,
    FlameRewardAdInternalStateClosed,
    FlameRewardAdInternalStateFailed,
    FlameRewardAdInternalStateDestroyed,
};

/// 内部事件观察者（A4A Custom Unlock Bridge 专用；不属于 Public API）。
/// 实现 FlameRewardListener 的对象可同时实现本协议，接收不进 Public 回调面的内部事件。
@protocol FlameRewardInternalObserver <FlameRewardListener>
@optional
/// 用户点击跳过（GroMore DidClickSkip；Public 面无对应事件）。
- (void)flame_rewardAdDidSkip;
/// 服务端奖励验证失败（verify==NO；A2 语义：终态错误由 close/error 通道表达，此通知仅供桥接区分原因）。
- (void)flame_rewardAdVerifyFailed:(nullable NSString *)reason;
@end

/// FlameRewardAd 协议的防护基类：
/// - 状态机 + 重复 load 短路（Loading 中不重复发起）
/// - show 前置 isReady 检查，未就绪同步 onAdError(14111)
/// - onAdReward / onAdClosed / onAdError 终态防重（once-only）
/// - 全部 Listener 回调与状态变更统一在主线程执行
/// - destroy 后阻断一切回调与三方请求
/// - stale 事件按「当前代际」过滤（每次 load 递增 generation）
/// 子类（GroMore 适配器、测试 Fake）只需实现 template hooks 并在收到三方事件时调用 flame_event*。
@interface FlameRewardAdBase : NSObject <FlameRewardAd>

@property (nonatomic, assign, readonly) FlameRewardAdInternalState internalState;
@property (nonatomic, strong, readonly, nullable) FlameEcpmInfo *showEcpmInfo;
@property (nonatomic, strong, readonly, nullable) FlameEcpmInfo *bestEcpmInfo;
@property (nonatomic, assign, readonly) NSInteger loadGeneration;
@property (nullable, nonatomic, weak, readonly) id<FlameRewardListener> listener;
@property (nullable, nonatomic, weak, readonly) UIViewController *viewController;

- (instancetype)initWithViewController:(nullable UIViewController *)viewController
                              listener:(nullable id<FlameRewardListener>)listener NS_DESIGNATED_INITIALIZER;

#pragma mark - Subclass hooks

/// 发起新一轮加载（基类保证：仅在非 Loading/Destroyed 状态调用，且 generation 已递增）。
- (void)flame_loadWithUserId:(nullable NSString *)userId customData:(nullable NSString *)customData;

/// 三方真实就绪状态（供 isReady）。
- (BOOL)flame_providerIsReady;

/// 三方真实加载中状态（供 isLoading）。
- (BOOL)flame_providerIsLoading;

/// 从指定 VC 展示（基类保证 isReady 已通过；返回 NO 时子类必须调用 flame_eventShowFailed）。
- (void)flame_showFromViewController:(nullable UIViewController *)viewController;

/// 释放三方对象、置空 delegate。基类已标记 Destroyed。
- (void)flame_destroyInternal;

#pragma mark - Event feed-ins（可在任意线程调用；子类负责确认事件属于当前代际对象）

- (void)flame_eventLoaded;
- (void)flame_eventLoadFailed:(NSString *)code desc:(NSString *)desc;
- (void)flame_eventWillShow;
- (void)flame_eventShown;
- (void)flame_eventShowFailed:(NSString *)code desc:(NSString *)desc;
- (void)flame_eventClicked;
- (void)flame_eventPlayCompleted;
- (void)flame_eventPlayFailed:(NSString *)code desc:(NSString *)desc;
/// verified==YES 时向 Host 派发一次 onAdReward（once-only）；NO 时不派发成功奖励。
- (void)flame_eventRewardVerified:(BOOL)verified
                            userId:(nullable NSString *)userId
                        customData:(nullable NSString *)customData
                            transId:(nullable NSString *)transId;
/// 内部可感知；不触发 onAdReward（Content 映射留给 A3）。
- (void)flame_eventSkipped;
- (void)flame_eventClosed;

/// 子类在曝光/加载成功时机回填 eCPM。
- (void)flame_setShowEcpmInfo:(nullable FlameEcpmInfo *)info;
- (void)flame_setBestEcpmInfo:(nullable FlameEcpmInfo *)info;

/// 事件是否属于当前（未过期、未销毁）会话；子类在转发三方事件前应检查。
- (BOOL)flame_isEventAcceptable;

/// 主线程执行辅助（子类可用）。
- (void)flame_onMain:(dispatch_block_t)block;

@end

NS_ASSUME_NONNULL_END
