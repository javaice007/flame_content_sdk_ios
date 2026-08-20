#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "FlameUnlockSession.h"

NS_ASSUME_NONNULL_BEGIN

/// /sdk/init scenePIds 的解锁场景键（A4A 冻结；后端契约字段）。
FOUNDATION_EXPORT NSString * const FlameUnlockSceneContentUnlock;  // "content_unlock"

/// Content 回传的 cpm 值（官方 28261：无法回传时允许空串；此处为空串常量）。
FOUNDATION_EXPORT NSString * const FlameUnlockCpmEmptyValue;

/// A4A Custom Unlock Bridge 核心（纯 Foundation，不含任何 DJX/GroMore 具体类型；模拟器可测）。
///
/// 职责：一个 Content showCustomAD 流程 ↔ 一次内部 Direct GroMore Reward 请求的桥接：
/// 场景解析（scenePIds.content_unlock → Flame Placement → pIds → slotID）→ 复用
/// FlameContentSdk 内部 Reward 创建链（load → show → 曝光/eCPM/Reward/Skip/Error/Close 映射）。
/// 不判断内容是否解锁：最终解锁结果由 Content SDK 在 unlockFlowEnd 权威给出。
///
/// 线程：DJX/GroMore 回调线程不可信 → 入口与事件统一主线程；Content 回调主线程执行。
@interface FlameCustomUnlockBridge : NSObject <FlameUnlockSessionDelegate>

+ (instancetype)sharedBridge;

/// 内部 Reward 创建 seam（默认走 FlameContentSdk 创建链：placement 映射 + Provider 工厂）。
/// 参数：presenter VC、Flame Placement ID、listener（session）。返回 nil = 创建失败。
@property (nonatomic, copy, nullable) id<FlameRewardAd> _Nullable (^rewardAdFactory)(UIViewController *_Nullable presenter,
                                                                                     NSString *flamePlacementId,
                                                                                     id<FlameRewardListener> listener);

/// Presenter 解析 seam（默认复用 A2 顶层 VC 解析策略；找不到 = 明确失败 14133）。
@property (nonatomic, copy, nullable) UIViewController *_Nullable (^presenterProvider)(void);

/// unlockFlowStart（官方 28261：无自定义弹窗可直接回传广告解锁方案，SDK 展示默认弹窗）。
/// Flame 无自渲染弹窗需求 → echo 解锁方案（playletId 来自 Content 流程；episodeCount 来自
/// 页面创建时的 DJXPlayletConfig，由 DJX adapter 捕获传入）。Host 完全无参与。
- (void)unlockFlowDidStartWithPlayletId:(NSInteger)playletId
                           episodeCount:(NSInteger)episodeCount
                        completionHandler:(void (^)(NSInteger planPlayletId, NSInteger planEpisodeCount, BOOL cancel))handler;

/// showCustomAD 核心。flowKey 由 Content adapter 生成（流程标识）。
/// onADWillShow：广告真实曝光时回调（GroMore DidVisible），至多一次；cpm 为 GroMore 原始串（分），
///               不可用（nil/空/无权限 "-3"）时按官方口径回传空串。
/// onADRewardDidVerified：广告阶段终态回调，至多一次（success=奖励验证成功；extraData 只含真实数据）。
- (void)showCustomADForFlowKey:(NSString *)flowKey
                      playletId:(NSInteger)playletId
                    onADWillShow:(void (^_Nullable)(NSString *cpm))onADWillShow
             onADRewardDidVerified:(void (^)(BOOL success, NSString *_Nullable cpm, NSDictionary<NSString *, id> *_Nullable extraData))onADRewardDidVerified;

/// unlockFlowEnd：Content 侧流程收口事实 → 清理对应 Session/广告对象；不二次回调、不维护权益。
- (void)unlockFlowDidEndForFlowKey:(NSString *)flowKey
                           success:(BOOL)success
                       errorDomain:(nullable NSString *)errorDesc;

@end

NS_ASSUME_NONNULL_END
