#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class FlameUnlockSession;

/// 解锁会话注册表：showCustomAD 流程 ↔ 内部 Reward 请求的关联查表。
/// 严格不是 entitlement DB / 集数数据库 / 内容权限中心（A4A 边界，docs/context/自定义解锁/02）。
@interface FlameUnlockSessionRegistry : NSObject

+ (instancetype)sharedRegistry;

/// 注册已有会话（返回注册后的会话本身，便于链式）。
- (FlameUnlockSession *)registerSession:(FlameUnlockSession *)session;

- (nullable FlameUnlockSession *)sessionForId:(NSString *)sessionId;

/// 同一 Content 流程（flowKey）最近一个未收口（非 Finished）会话；unlockFlowEnd 用。
- (nullable FlameUnlockSession *)latestUnfinishedSessionForFlowKey:(NSString *)flowKey;

- (void)removeSession:(FlameUnlockSession *)session;

/// 当前未收口会话数（诊断/测试）。
- (NSUInteger)liveSessionCount;

/// FlameContentSdk.clear 时全量清理（A3 已接线）。
- (void)removeAllSessions;

@end

NS_ASSUME_NONNULL_END
