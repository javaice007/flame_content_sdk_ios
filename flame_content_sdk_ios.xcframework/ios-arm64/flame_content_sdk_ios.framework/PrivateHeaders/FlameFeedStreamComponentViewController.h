#import <UIKit/UIKit.h>
#import "../Public/FlameContentAuxiliary.h"

NS_ASSUME_NONNULL_BEGIN

/// Flame 自有沉浸式竖向 Feed UI（0.4.5 Lite：静态封面态，无内嵌播放）。
/// 每 item 占满视口；可见性几何判定经 onDominantCandidate 上报给组件。
@interface FlameFeedStreamComponentViewController : UIViewController

@property (nonatomic, copy, nullable) NSArray<FlameContentPlaylet *> *(^viewModelsBlock)(void);

/// 滚动几何上报：主导候选（可见占比最大且 ≥50% 的 item）与其可见比例；无主导为 nil。
@property (nonatomic, copy, nullable) void (^onDominantCandidate)(FlameContentPlaylet *_Nullable item,
                                                                  CGFloat ratio);
@property (nonatomic, copy, nullable) void (^onLoadMoreTriggered)(void);
@property (nonatomic, copy, nullable) void (^onRefreshTriggered)(void);
@property (nonatomic, copy, nullable) void (^onSelectItem)(NSUInteger index);
@property (nonatomic, copy, nullable) void (^onVisibilityChanged)(BOOL visible);
@property (nonatomic, copy, nullable) void (^onAttached)(void);
@property (nonatomic, copy, nullable) void (^onReleased)(void);

- (void)flame_reloadData;
- (void)flame_setRefreshing:(BOOL)refreshing;
- (void)flame_setLoadingMore:(BOOL)loadingMore;
- (void)flame_showError:(nullable NSString *)desc;
/// active item 高亮（ActiveItemManager 提交后驱动）。
- (void)flame_setActiveItem:(nullable FlameContentPlaylet *)item;

@end

NS_ASSUME_NONNULL_END
