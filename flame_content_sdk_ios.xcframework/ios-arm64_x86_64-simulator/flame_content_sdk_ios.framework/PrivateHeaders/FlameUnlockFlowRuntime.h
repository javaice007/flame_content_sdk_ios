#import <Foundation/Foundation.h>
#import "../Public/FlameContentEntry.h"
#import "FlameUnlockFlowSession.h"

@class FlamePlayerRuntime;

NS_ASSUME_NONNULL_BEGIN

/// Unlock Flow 错误码（142xx 组件段；0.4.0 登记）。
extern NSString *const FlameUnlockFlowErrorCodeBusy;              ///< 14214 已有活跃解锁流程（fail closed）
extern NSString *const FlameUnlockFlowErrorCodeRewardNotVerified; ///< 14215 Reward 未验证成功（非裁决）
extern NSString *const FlameUnlockFlowErrorCodeContentUpdateFailed;///< 14216 Content State Update 回报失败

/// 事件出口（内部注册；组件创建时由 FlameComponentRuntime 注入，弱持有组件）。
/// 事件枚举 `FlameUnlockFlowEvent` 的公开声明位于 FlameContentEntry.h（24 号文档载体结论）。
typedef void (^FlameUnlockFlowEventSink)(FlameUnlockFlowEvent event,
                                         BOOL unlocked,
                                         NSString *_Nullable code);

/// Unlock Flow Runtime（内部单例；0.4.0 `23_UNLOCK_FLOW_SCOPE.md` 落地）。
///
/// 定位：Content unlock 流程的**编排与可观化层**——插在 FlameCSJCustomUnlockAdapter 与
/// FlameCustomUnlockBridge 之间（23 号 §5 冻结接缝），只观察与转述，不裁决：
/// - Bridge/Registry/AttachCenter/Reward/Login 零修改；
/// - `Reward Success != Unlock Success`：`ContentUpdated(unlocked=YES)` 只能由
///   `flame_flowDidEndWithContentUpdateSuccess:YES`（真实 Content 流程收口事实）触发；
/// - 单活跃 Flow：并发请求 fail closed（14214），底层真实链路不受影响；
/// - 极简可选内部 UI（解锁中覆盖层），按注册的 uiEnabled 决定；
/// - 事件载荷仅枚举 / 布尔 / 规范化错误码（24 号 §3 脱敏边界）。
@interface FlameUnlockFlowRuntime : NSObject

+ (instancetype)sharedRuntime;

@property (nonatomic, readonly, nullable) FlameUnlockFlowSession *activeSession;

/// 由 FlameComponentRuntime 注入（读取活跃 Player Session 做诊断关联快照）。
@property (nonatomic, weak, nullable) FlamePlayerRuntime *playerRuntime;

/// 组件创建时注册事件出口；uiEnabled = 该组件 options 的 unlockFlowUIEnabled。
- (void)flame_registerOwnerWithComponentId:(NSString *)componentId
                                  uiEnabled:(BOOL)uiEnabled
                                       sink:(FlameUnlockFlowEventSink)sink;

- (void)flame_unregisterOwnerWithComponentId:(NSString *)componentId;

#pragma mark - 集成接缝（FlameCSJCustomUnlockAdapter 调用；内部主线程归一）

/// Content unlock 流程开始（对应 playletDetailUnlockFlowStart）。
- (void)flame_flowDidStart;

/// 进入 Reward 加载（对应 showCustomAD 入口）。
- (void)flame_flowDidRequestReward;

/// Reward 真实曝光（对应 onADWillShow）。
- (void)flame_flowRewardWillShow;

/// Reward 终结（对应 onADRewardDidVerified；success=NO → 事件 code 14215，非裁决）。
- (void)flame_flowRewardSettledWithSuccess:(BOOL)success;

/// Content 流程收口（对应 playletDetailUnlockFlowEnd；唯一 unlocked=YES 来源）。
- (void)flame_flowDidEndWithContentUpdateSuccess:(BOOL)success;

#pragma mark - 测试

- (BOOL)flame_overlayVisibleForTesting;

+ (void)flame_resetForTesting;

@end

NS_ASSUME_NONNULL_END
