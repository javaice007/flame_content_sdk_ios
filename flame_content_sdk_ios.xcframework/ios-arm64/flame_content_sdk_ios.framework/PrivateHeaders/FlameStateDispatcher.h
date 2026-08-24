#import <Foundation/Foundation.h>
#import "../Public/FlameContentEntry.h"

@class FlameComponentRegistry;

NS_ASSUME_NONNULL_BEGIN

/// 状态转换唯一通道（Runtime Core 设计 §2.4）。
/// 职责：转换合法性校验（非法转换断言 + 返回 NO，不静默）、更新 Registry 元数据、
/// 主线程执行通知回调。回调不可重入：派发期间组件再次发起的转换由组件在下一轮处理。
@interface FlameStateDispatcher : NSObject

- (instancetype)initWithRegistry:(FlameComponentRegistry *)registry;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

+ (BOOL)canTransitionFrom:(FlameComponentState)from to:(FlameComponentState)to;

/// 校验并应用转换。previousState → newState 不合法时记录错误、断言并返回 NO（fail closed）。
/// notifyBlock 同步在主线程执行（listener 回调入口）。
- (BOOL)applyState:(FlameComponentState)newState
         component:(id)component
        identifier:(NSString *)componentId
     previousState:(FlameComponentState)previousState
       notifyBlock:(nullable void (^)(void))notifyBlock;

@end

NS_ASSUME_NONNULL_END
