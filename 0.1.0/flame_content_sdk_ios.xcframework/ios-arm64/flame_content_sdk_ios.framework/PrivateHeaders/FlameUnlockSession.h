#import <Foundation/Foundation.h>
#import "../ads/FlameRewardAdBase.h"

NS_ASSUME_NONNULL_BEGIN

@class FlameUnlockSession;

/// 解锁会话状态机（主线程驱动；A4A 设计见 docs/context/自定义解锁/02_UNLOCK_SESSION_DESIGN.md）。
/// 主轴是「广告桥接生命周期」，不是「是否解锁成功」——解锁结果永远归 Content SDK（unlockFlowEnd）。
typedef NS_ENUM(NSInteger, FlameUnlockSessionState) {
    FlameUnlockSessionStateCreated = 0,   // showCustomAD 入口已建会话
    FlameUnlockSessionStateResolving,     // 场景/广告位/Presenter 解析中
    FlameUnlockSessionStateLoading,       // 内部 Reward 加载中
    FlameUnlockSessionStateLoaded,        // 加载完成（待展示）
    FlameUnlockSessionStateShowing,       // 展示请求已发
    FlameUnlockSessionStateExposed,       // 广告曝光（onADWillShow 已回传，至多一次）
    FlameUnlockSessionStateRewarded,      // 奖励验证成功（verify==YES，结果已回传）
    FlameUnlockSessionStateSkipped,       // 用户跳过（结果已按官方契约回传 success=NO）
    FlameUnlockSessionStateFailed,        // 广告链路失败（load/show/create 等，结果已回传）
    FlameUnlockSessionStateClosed,        // 广告关闭且未达成奖励（结果已回传 success=NO）
    FlameUnlockSessionStateFinished,      // unlockFlowEnd 收口，资源已清理
};

/// Session → Bridge 事件委托（主线程；由 FlameRewardAdBase 归一）。
@protocol FlameUnlockSessionDelegate <NSObject>
- (void)unlockSessionDidReceiveLoaded:(FlameUnlockSession *)session;
- (void)unlockSessionDidReceiveShown:(FlameUnlockSession *)session;
- (void)unlockSessionDidReceiveSkip:(FlameUnlockSession *)session;
- (void)unlockSessionDidReceiveVerifyFailed:(FlameUnlockSession *)session;
- (void)unlockSession:(FlameUnlockSession *)session
    didReceiveRewardUserId:(nullable NSString *)userId
                customData:(nullable NSString *)customData
                   transId:(nullable NSString *)transId;
- (void)unlockSessionDidReceiveClosed:(FlameUnlockSession *)session;
- (void)unlockSession:(FlameUnlockSession *)session didReceiveError:(nullable NSString *)code desc:(nullable NSString *)desc;
@end

/// 一个 Content showCustomAD 流程 ↔ 一个内部 GroMore Reward 请求的关联体。
/// 职责严格限定为广告桥接会话；不承担内容权益/解锁集数/购买记录。
@interface FlameUnlockSession : NSObject <FlameRewardInternalObserver>

@property (nonatomic, weak, nullable) id<FlameUnlockSessionDelegate> eventDelegate;
/// Content 回传 block（由 Bridge 在 showCustomAD 入口注入；一次性、主线程执行）。
@property (nonatomic, copy, nullable) void (^onADWillShowBlock)(NSString *cpm);
@property (nonatomic, copy, nullable) void (^onADRewardResultBlock)(BOOL success, NSString *_Nullable cpm, NSDictionary<NSString *, id> *_Nullable extraData);
@property (nonatomic, copy, readonly) NSString *sessionId;
@property (nonatomic, assign, readonly) FlameUnlockSessionState state;
@property (nonatomic, copy, readonly) NSString *flowKey;                    // Content 流程标识（DJX adapter 生成）
@property (nonatomic, assign, readonly) NSInteger playletId;                // 诊断
@property (nonatomic, copy, readonly, nullable) NSString *flamePlacementId;
@property (nonatomic, copy, readonly, nullable) NSString *externalSlotId;
@property (nonatomic, strong, readonly, nullable) id<FlameRewardAd> rewardAd;
@property (nonatomic, copy, readonly, nullable) NSString *cpmRawValue;      // 曝光 eCPM 原始串（分）
@property (nonatomic, readonly) BOOL adPhaseTerminal;                       // 广告阶段已终态（Content 结果至多回传一次）
@property (nonatomic, readonly) BOOL willShowDelivered;
@property (nonatomic, readonly) BOOL rewardResultDelivered;
@property (nonatomic, readonly) BOOL verifyFailed;
@property (nonatomic, readonly) NSDate *createdAt;
@property (nonatomic, readonly, nullable) NSDate *terminalAt;

- (instancetype)initWithSessionId:(NSString *)sessionId
                          flowKey:(NSString *)flowKey
                         playletId:(NSInteger)playletId;

// 状态迁移（仅主线程；由 Bridge 调用）
- (void)flame_setState:(FlameUnlockSessionState)state;
- (void)flame_setPlacement:(nullable NSString *)flamePlacementId slot:(nullable NSString *)slotId;
- (void)flame_setRewardAd:(nullable id<FlameRewardAd>)rewardAd;
- (void)flame_setCpmRawValue:(nullable NSString *)rawValue;
- (void)flame_markWillShowDelivered;
- (void)flame_markRewardResultDelivered;
- (void)flame_markVerifyFailed;

/// 事件是否可被本会话接受（Finished 后拒绝；Bridge 按 listener 归属路由为第一防线）。
- (BOOL)flame_acceptsEvents;

@end

NS_ASSUME_NONNULL_END
