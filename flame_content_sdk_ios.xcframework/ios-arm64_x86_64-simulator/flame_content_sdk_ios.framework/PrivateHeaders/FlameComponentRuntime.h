#import <Foundation/Foundation.h>
#import "FlameComponentAdapters.h"

NS_ASSUME_NONNULL_BEGIN

@class FlameComponentRegistry, FlameStateDispatcher, FlameNavigationCoordinator, FlameCapabilityResolver;
@class FlameFeedComponent, FlameFeedComponentOptions;

/// Component Runtime Core（内部单例；Runtime Core 设计 §2.1）。
///
/// - 主线程 confined：创建、状态、路由、能力查询均要求主线程（断言）；
/// - Host 独占组件强引用；Runtime 经 Registry 弱引用，不延长组件生命周期；
/// - 不持有内容权益、Login 状态、"当前页面 / 最后 Config / 当前 owner"或 Vendor 实例；
/// - Adapter 是唯一 Vendor 接入边界；缺省懒加载 CSJ 实现，可被测试替换。
@interface FlameComponentRuntime : NSObject

+ (instancetype)sharedRuntime;

@property (nonatomic, readonly) FlameComponentRegistry *registry;
@property (nonatomic, readonly) FlameStateDispatcher *stateDispatcher;
@property (nonatomic, readonly) FlameNavigationCoordinator *navigationCoordinator;
@property (nonatomic, readonly) FlameCapabilityResolver *capabilityResolver;

/// Adapter 安装点（内部 / 测试 seam）。置 nil 后再次读取会重建缺省 CSJ 实现。
@property (nonatomic, strong, nullable) id<FlameContentDataAdapter> contentDataAdapter;
@property (nonatomic, strong, nullable) id<FlamePlayerAdapter> playerAdapter;
@property (nonatomic, strong, nullable) id<FlameBindingAdapter> bindingAdapter;

/// 创建 Feed 组件（MVP 单实例：已存在存活 Feed 时返回 nil）。
/// 调用前须已完成能力协商；由公开 Facade 归一主线程后进入。
- (nullable FlameFeedComponent *)createFeedComponentWithOptions:(nullable FlameFeedComponentOptions *)options;

/// 组件析构时的注销钩子（组件 dealloc 调用；幂等）。
- (void)componentDidDealloc:(NSString *)componentId;

/// 组件进入 Released 的注销钩子（路由注销 + 注册表移除；幂等）。
- (void)componentDidRelease:(NSString *)componentId;

/// 测试 seam：重建注册表/派发器/路由并清空 Adapter 安装。
/// componentId 序列不复位（进程内永不复用）。
+ (void)flame_resetForTesting;

@end

NS_ASSUME_NONNULL_END
