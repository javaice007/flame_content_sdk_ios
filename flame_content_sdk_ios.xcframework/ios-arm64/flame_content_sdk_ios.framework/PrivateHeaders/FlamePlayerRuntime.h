#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class FlameComponentRuntime, FlameNavigationRouteRequest, FlamePlayerSession;

NS_ASSUME_NONNULL_BEGIN

/// Player Runtime（内部；0.3.4 `12_PLAYER_RUNTIME_DESIGN.md` 落地）。
///
/// feed/theater 路由 fulfillment 的统一呈现通道：
/// - 单活跃会话；活跃期间新请求 fail closed（14212，不排队、不自动顶替）；
/// - owner 以 weak 引用存活校验：owner 已释放时 late callback 一律丢弃；
/// - dismiss 检测（16 号文档 P2 修订案，见实现报告）：惰性对账——新请求进入与
///   App 前台钩子时检查 presenter 呈现链，已解除则旧会话标记 Dismissed；
/// - 会话状态只描述呈现，不含播放控制/进度/集数（P1 能力位保持 NO）；
/// - 不触碰 Unlock/Reward/Login/AttachCenter（显式 Detail 路径既有挂接原样生效）。
@interface FlamePlayerRuntime : NSObject

@property (nonatomic, readonly, nullable) FlamePlayerSession *activeSession;

- (instancetype)initWithRuntime:(FlameComponentRuntime *)runtime;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// 统一呈现入口（由 FlameComponentRuntime 的路由 handler 调用；主线程）。
/// owner 为发起路由的组件对象（弱引用持有）；presenter 通常为组件 rootViewController。
- (void)requestPresentationForRouteRequest:(FlameNavigationRouteRequest *)request
                                      owner:(nullable id)owner
                                  presenter:(nullable UIViewController *)presenter
                                completion:(nullable void (^)(NSString *_Nullable code,
                                                              NSString *_Nullable desc))completion;

@end

NS_ASSUME_NONNULL_END
