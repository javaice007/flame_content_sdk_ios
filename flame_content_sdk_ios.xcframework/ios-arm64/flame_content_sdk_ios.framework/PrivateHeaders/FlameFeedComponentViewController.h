#import <UIKit/UIKit.h>
#import "../Public/FlameContentAuxiliary.h"

NS_ASSUME_NONNULL_BEGIN

/// Flame 自有 Feed UI（内部；0.3.2 MVP = 海报网格列表，无内嵌自动播放）。
/// 数据与行为全部经 block 由 FlameFeedComponent 驱动；不接触 Vendor 类型。
@interface FlameFeedComponentViewController : UIViewController

/// 当前列表数据（主线程读取）。
@property (nonatomic, copy, nullable) NSArray<FlameContentPlaylet *> *(^viewModelsBlock)(void);

@property (nonatomic, copy, nullable) void (^onRefreshTriggered)(void);
@property (nonatomic, copy, nullable) void (^onLoadMoreTriggered)(void);
@property (nonatomic, copy, nullable) void (^onSelectItem)(NSUInteger index);
@property (nonatomic, copy, nullable) void (^onVisibilityChanged)(BOOL visible);
@property (nonatomic, copy, nullable) void (^onAttached)(void);
@property (nonatomic, copy, nullable) void (^onReleased)(void);

/// 数据变化后刷新列表与覆盖层（空态仅在非加载、非错误时展示）。
- (void)flame_reloadData;
- (void)flame_setRefreshing:(BOOL)refreshing;
- (void)flame_setLoadingMore:(BOOL)loadingMore;
/// 请求级错误展示（带重试；重试触发 onRefreshTriggered）。
- (void)flame_showError:(nullable NSString *)desc;

@end

NS_ASSUME_NONNULL_END
