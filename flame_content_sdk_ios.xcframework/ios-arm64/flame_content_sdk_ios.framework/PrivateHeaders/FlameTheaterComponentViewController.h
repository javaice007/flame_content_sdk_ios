#import <UIKit/UIKit.h>
#import "../Public/FlameContentAuxiliary.h"

NS_ASSUME_NONNULL_BEGIN

/// Flame 自有 Theater UI（内部；16 号 T4/T5：复制 Feed 实现模式，不改 Feed 文件）。
/// 结构：搜索栏 + 分类 chips + 海报网格 + 空态/错误覆盖层；数据与行为经 block 由组件驱动。
@interface FlameTheaterComponentViewController : UIViewController

/// 当前列表数据 / 分类 / 选中分类 / 搜索态（主线程读取）。
@property (nonatomic, copy, nullable) NSArray<FlameContentPlaylet *> *(^viewModelsBlock)(void);
@property (nonatomic, copy, nullable) NSArray<NSString *> *(^categoriesBlock)(void);
@property (nonatomic, copy, nullable) NSString *_Nullable (^selectedCategoryBlock)(void);
@property (nonatomic, copy, nullable) BOOL (^searchActiveBlock)(void);

@property (nonatomic, copy, nullable) void (^onSelectCategory)(NSString *category);
@property (nonatomic, copy, nullable) void (^onSearch)(NSString *keyword);
@property (nonatomic, copy, nullable) void (^onSearchCleared)(void);
@property (nonatomic, copy, nullable) void (^onRefreshTriggered)(void);
@property (nonatomic, copy, nullable) void (^onLoadMoreTriggered)(void);
@property (nonatomic, copy, nullable) void (^onSelectItem)(NSUInteger index);
@property (nonatomic, copy, nullable) void (^onVisibilityChanged)(BOOL visible);
@property (nonatomic, copy, nullable) void (^onAttached)(void);
@property (nonatomic, copy, nullable) void (^onReleased)(void);

/// 数据变化后刷新网格 / chips / 覆盖层。
- (void)flame_reloadData;
- (void)flame_setRefreshing:(BOOL)refreshing;
- (void)flame_setLoadingMore:(BOOL)loadingMore;
/// 请求级错误展示（带重试；重试触发 onRefreshTriggered）。
- (void)flame_showError:(nullable NSString *)desc;

@end

NS_ASSUME_NONNULL_END
