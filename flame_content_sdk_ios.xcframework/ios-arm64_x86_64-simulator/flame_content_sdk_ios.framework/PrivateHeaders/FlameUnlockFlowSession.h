#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Unlock Flow 状态（0.4.0 `23_UNLOCK_FLOW_SCOPE.md` §2 冻结）。
/// 只描述流程进展；不表示权益：`Unlocked` 仅转述真实 Content State Update 结果。
typedef NS_ENUM(NSInteger, FlameUnlockFlowState) {
    FlameUnlockFlowStateRequested = 0,
    FlameUnlockFlowStateRewardLoading,
    FlameUnlockFlowStateRewardShowing,
    FlameUnlockFlowStateRewardSettled,
    FlameUnlockFlowStateContentStateUpdating,
    FlameUnlockFlowStateUnlocked,
    FlameUnlockFlowStateUnlockFailed,
};

/// 一次解锁流程的不可变上下文（内部；仅诊断）。
///
/// 字段白名单（23 号 §3 冻结）：flowSessionId / playerSessionId / ownerComponentId / state。
/// 禁止增加：playletId、用户信息、权益字段、placement、transId。
/// flowSessionId 形如 `uf-<单调序列>`，永不复用；playerSessionId 为创建时活跃 Player
/// Session 的只读快照（可为空）；三者仅用于日志/事件归因，不写入任何 Registry。
@interface FlameUnlockFlowSession : NSObject

@property (nonatomic, copy, readonly) NSString *flowSessionId;
@property (nonatomic, copy, readonly, nullable) NSString *playerSessionId;
@property (nonatomic, copy, readonly, nullable) NSString *ownerComponentId;
/// 内部可写：仅 FlameUnlockFlowRuntime 驱动流转。
@property (nonatomic, assign) FlameUnlockFlowState state;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

- (nullable instancetype)initWithFlowSessionId:(NSString *)flowSessionId
                               playerSessionId:(nullable NSString *)playerSessionId
                              ownerComponentId:(nullable NSString *)ownerComponentId;

@end

NS_ASSUME_NONNULL_END
