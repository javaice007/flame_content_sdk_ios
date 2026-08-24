#import <Foundation/Foundation.h>
#import "FlameComponentContext.h"
#import "../Public/FlameContentEntry.h"

NS_ASSUME_NONNULL_BEGIN

/// 组件注册表（Runtime Core 设计 §2.3）。
/// 以弱引用持有组件：Host 独占强引用，Registry 不延长组件生命周期。
/// 绑定表为本类附属：仅接受 Binding Adapter 验证成功的令牌（0.3.2 现实实现为 fail closed，
/// 绑定表恒为空）。
@interface FlameComponentRegistry : NSObject

- (void)registerComponent:(id)component
               identifier:(NSString *)componentId
                     kind:(FlameComponentKind)kind;

- (void)unregisterComponentWithIdentifier:(NSString *)componentId;

- (nullable NSString *)componentIdForComponent:(id)component;

- (void)updateState:(FlameComponentState)state forIdentifier:(NSString *)componentId;

- (BOOL)hasLiveComponentOfKind:(FlameComponentKind)kind;

- (NSUInteger)liveComponentCount;

- (void)storeBindingToken:(nullable NSString *)token forIdentifier:(NSString *)componentId;

- (nullable NSString *)bindingTokenForIdentifier:(NSString *)componentId;

- (void)removeAllEntries;

@end

NS_ASSUME_NONNULL_END
