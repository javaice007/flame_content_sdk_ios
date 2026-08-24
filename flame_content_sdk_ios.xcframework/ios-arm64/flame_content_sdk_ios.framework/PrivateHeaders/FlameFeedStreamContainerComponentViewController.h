#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/// Vendor Stream Container 薄壳 VC（0.4.10；49 号 §4 三层结构的 Flame 层）。
///
/// 职责单一：承载既有 FlameContentContainerViewController（0.2.0 容器壳，零修改复用——
/// 其内部完成 DrawVideo 单实例 gate 占位、appear/disappear 生命周期桥接与前后台通知），
/// 并把宿主 containment 事件回传组件（与 FlameFeedStreamComponentViewController 同一接缝）。
/// 不绘制任何内容 UI（内容本体 = Vendor DrawVideo 页面）。
@interface FlameFeedStreamContainerComponentViewController : UIViewController

/// 组件回调接缝（与 Lite VC 同型；组件在创建时注入）。
@property (nonatomic, copy, nullable) void (^onAttached)(void);
@property (nonatomic, copy, nullable) void (^onVisibilityChanged)(BOOL visible);
@property (nonatomic, copy, nullable) void (^onReleased)(void);
@property (nonatomic, copy, nullable) void (^onViewportChanged)(CGSize size,
                                                            CGFloat topInset,
                                                            CGFloat bottomInset);

/// 先创建空壳；首个有效 viewport 到达后由 Component 安装 0.2.0 容器壳。
- (instancetype)init;

/// 安装 0.2.0 容器壳（仅内部 Component 在首个有效 viewport 后调用）。
- (BOOL)installContainerViewController:(UIViewController *)containerViewController;

@end

NS_ASSUME_NONNULL_END
