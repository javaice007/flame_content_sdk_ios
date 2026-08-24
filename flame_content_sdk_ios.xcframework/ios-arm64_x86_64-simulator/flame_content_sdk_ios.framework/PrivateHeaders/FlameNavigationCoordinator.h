#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Feed → Player 显式路由（0.3.2 MVP 唯一注册的路由）。
extern NSString *const FlameNavigationRouteFeedToPlayer;

/// 一次路由请求（不可变值对象）。
@interface FlameNavigationRouteRequest : NSObject

@property (nonatomic, copy, readonly) NSString *routeIdentifier;
@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) NSInteger playletId;
@property (nonatomic, readonly) NSInteger episode;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

+ (instancetype)flame_requestWithRoute:(NSString *)routeIdentifier
                            componentId:(NSString *)componentId
                               playletId:(NSInteger)playletId
                                 episode:(NSInteger)episode;

@end

typedef void (^FlameNavigationHandlerBlock)(FlameNavigationRouteRequest *request);

/// 显式路由协调器（Runtime Core 设计 §2.5）。
/// 组件不直接创建彼此：导航请求经 Coordinator 分发给已注册 handler；
/// 未注册 handler 的路由 fail closed（回调 14201），不猜测默认目标。
@interface FlameNavigationCoordinator : NSObject

- (void)registerHandler:(FlameNavigationHandlerBlock)handler forRoute:(NSString *)route;

- (void)unregisterRoute:(NSString *)route;

- (void)requestRoute:(FlameNavigationRouteRequest *)request
          completion:(void (^)(NSString *_Nullable code, NSString *_Nullable desc))completion;

@end

NS_ASSUME_NONNULL_END
