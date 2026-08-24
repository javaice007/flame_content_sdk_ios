#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameContentSdk.h>

@class FlameContentContainerViewController;

NS_ASSUME_NONNULL_BEGIN

/// Flame Content SDK 内容入口（A4C 冻结；三入口 VC 返回式）。
///
/// 契约：
/// - 须在 `init success`（FlameCallback.success）之后调用；SDK 未初始化、/sdk/init 未声明
///   content 或 Content 未 Ready 时返回 nil（可预测，不 crash，不触发第三方页面创建）。
/// - 返回的 UIViewController 由 Host 自行 push/present 并持有；Host 无需显式销毁，
///   页面释放后 Flame 不强持有（内部 delegates 均 weak）。
/// - Content 页面内部由 Flame 自动挂接 Custom Unlock（Host 不提供广告位、不设置任何
///   内容回调、不判断解锁）。
/// - 线程：任意线程可调用；SDK 内部归一主线程后同步返回（主线程调用直接执行）。
/// - Content 正式运行环境为真机 arm64（内容栈无模拟器切片；模拟器调用返回 nil）。
@interface FlameContentSdk (ContentEntry)

/// 创建内容聚合页（短剧列表/推荐聚合入口）。
+ (nullable UIViewController *)contentAggregatePage;

/// 创建指定短剧详情页（高级入口：Host 已知业务 skitId 时使用；普通客户可用 Aggregate/Feed）。
/// skitId 为 Flame/Content 短剧业务 ID，不是第三方广告位 ID；skitId <= 0 返回 nil。
/// episode <= 0 时不主动设置集序（保持 Content SDK 默认起始行为，以其实际支持为准）；
/// episode > 0 表示期望进入的集序号（最终以当前正式 Content SDK 支持能力为准）。
+ (nullable UIViewController *)contentPlayletPageForSkitId:(NSInteger)skitId
                                                   episode:(NSInteger)episode;

/// 创建内容滑滑流/沉浸流入口。
+ (nullable UIViewController *)contentFeedPage;

/// 创建可嵌入 Host 子控制器树的 Feed Container。
///
/// 仅在 SDK 与 Content Ready 后创建；不可用时返回 nil。Host 必须使用 UIKit child
/// controller containment 持有此对象。Phase 1 同时仅支持一个存活的 Feed Container；
/// 当前实例释放前再次创建会返回 nil。
+ (nullable FlameContentContainerViewController *)createFeedContainerViewController;

@end

#pragma mark - Component SDK（0.3.2 Feed Component MVP）

/// 组件状态：只描述装载、可见性与可用性。
/// 不表示解锁成功、Reward 成功、内容权益或 Login 状态。
typedef NS_ENUM(NSInteger, FlameComponentState) {
    FlameComponentStateCreated = 0,
    FlameComponentStateAttached,
    FlameComponentStateVisible,
    FlameComponentStateHidden,
    FlameComponentStateBackgrounded,
    FlameComponentStateForegrounded,
    FlameComponentStateReleased,
    FlameComponentStateFailed,
};

/// 解锁流程脱敏事件（0.4.0 `24_UNLOCK_EVENT_API_REVIEW.md` 冻结语义）。
/// 仅状态转述，不含裁决权：Host 不得据此向 SDK 回写任何权益。
typedef NS_ENUM(NSInteger, FlameUnlockFlowEvent) {
    FlameUnlockFlowEventRequested = 0,   ///< 解锁流程已发起（unlocked 恒 NO；code 恒 nil）
    FlameUnlockFlowEventRewardSettled,   ///< Reward 生命周期终结（非裁决；code 非空表示 Reward 出错）
    FlameUnlockFlowEventContentUpdated,  ///< Content State Update 已回报（仅此事件 unlocked 参数有效）
    FlameUnlockFlowEventFailed,          ///< 流程失败（fail closed 等路径）
};

/// FlameFeedComponent 创建参数（MVP）。
/// freeEpisodeCount / unlockEpisodeCount 为组件级策略预留字段（<=0 使用全局默认）；
/// 0.3.2 生效策略仍为全局 `setContentUnlockConfigWithFreeEpisodeCount:unlockEpisodeCount:`，
/// 组件级覆盖随 Player Component 正式化落地。
@interface FlameFeedComponentOptions : NSObject

@property (nonatomic, assign) NSInteger freeEpisodeCount;
@property (nonatomic, assign) NSInteger unlockEpisodeCount;
/// 解锁流程极简内部 UI（加载覆盖层）开关；默认 YES。关闭仅隐藏展示，流程与事件不变。
@property (nonatomic, assign) BOOL unlockFlowUIEnabled;

@end

@class FlameFeedComponent;

/// Feed 组件事件回调（全部主线程；weak observer）。
@protocol FlameFeedComponentListener <NSObject>
@optional
/// 状态变化（created → attached → visible ↔ hidden / backgrounded ↔ foregrounded → released）。
- (void)feedComponent:(FlameFeedComponent *)component didChangeState:(FlameComponentState)state;
/// 请求级失败（数据请求失败、路由 fail closed 等；组件保持可用，可重试）。
- (void)feedComponent:(FlameFeedComponent *)component didFailWithCode:(NSString *)code desc:(NSString *)desc;
/// item 点击产生的 Feed → Player 导航请求（自动路由模式下仅作通知；Host 不处理呈现）。
- (void)feedComponent:(FlameFeedComponent *)component didRequestDetailForPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 解锁流程脱敏事件（0.4.0 @optional 增量；主线程；未实现的 Host 零影响）。
/// `ContentUpdated(unlocked=YES)` 只能来自真实 Content State Update（禁止模拟）。
- (void)feedComponent:(FlameFeedComponent *)component
didReceiveUnlockFlowEvent:(FlameUnlockFlowEvent)event
             unlocked:(BOOL)unlocked
                  code:(nullable NSString *)code;
@end

/// Flame 自有 Feed 组件（Flame UI；不包装 Vendor Feed Page）。
///
/// 契约：
/// - 经 `createFeedComponentWithOptions:` 创建；未初始化、Content 未 Ready、模拟器形态、
///   或已存在存活 Feed 组件时返回 nil（fail closed，不猜测降级）。
/// - Host 通过 `rootViewController` 执行 UIKit child containment 并独占持有；
///   组件状态机由 containment 与 App 前后台驱动，回调主线程。
/// - MVP 仅支持单实例：当前实例 Released 前再次创建返回 nil。
/// - `reload` / `loadMore` 驱动内部数据契约（分页）；item 点击经内部显式路由进入
///   播放页（复用既有 Content 显式 Detail 路径）；路由不可用时回调 14201（fail closed）。
/// - 组件状态与解锁/权益/Login 无关；Host 不获得 Vendor 类型、广告位或权益裁决。
@interface FlameFeedComponent : NSObject

@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) FlameComponentState state;
@property (nonatomic, weak, nullable) id<FlameFeedComponentListener> listener;
@property (nonatomic, readonly, nullable) UIViewController *rootViewController;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/// 首次 reload 在组件 attach 后自动发起；Host 可随时再次调用刷新。
- (void)reload;

/// 追加下一页（仅当上一页声明 hasMore；否则本调用为 no-op）。
- (void)loadMore;

@end

@interface FlameContentSdk (ComponentEntry)

/// 创建 Flame Feed 组件（0.3.2 MVP）。
/// 任意线程可调用；SDK 内部归一主线程后同步返回（主线程调用直接执行）。
/// 不可用形态返回 nil（语义同上）；正式运行环境为真机 arm64。
+ (nullable FlameFeedComponent *)createFeedComponentWithOptions:(nullable FlameFeedComponentOptions *)options;

@end

#pragma mark - Component SDK（0.3.6 Theater Component MVP）

@class FlameTheaterComponent;

/// Theater 组件创建参数（MVP）。
/// initialCategoryId 为分类名字符串（与内容分类列表返回一致）；nil = 默认/首个分类。
@interface FlameTheaterComponentOptions : NSObject

@property (nonatomic, copy, nullable) NSString *initialCategoryId;
/// 解锁流程极简内部 UI（加载覆盖层）开关；默认 YES。关闭仅隐藏展示，流程与事件不变。
@property (nonatomic, assign) BOOL unlockFlowUIEnabled;

@end

/// Theater 组件事件回调（全部主线程；weak observer）。
@protocol FlameTheaterComponentListener <NSObject>
@optional
/// 状态变化（与 FlameFeedComponent 同一状态机契约）。
- (void)theaterComponent:(FlameTheaterComponent *)component didChangeState:(FlameComponentState)state;
/// 请求级失败（分类/列表/搜索失败、路由 fail closed 等；组件保持可用，可重试）。
- (void)theaterComponent:(FlameTheaterComponent *)component didFailWithCode:(NSString *)code desc:(NSString *)desc;
/// item 点击产生的 Theater → Player 导航请求（自动路由模式下仅作通知；Host 不处理呈现）。
- (void)theaterComponent:(FlameTheaterComponent *)component didRequestDetailForPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 解锁流程脱敏事件（0.4.0 @optional 增量；语义同 Feed）。
- (void)theaterComponent:(FlameTheaterComponent *)component
didReceiveUnlockFlowEvent:(FlameUnlockFlowEvent)event
             unlocked:(BOOL)unlocked
                  code:(nullable NSString *)code;
@end

/// Flame 自有 Theater 组件（分类 chips + 海报网格 + 基础搜索；不包装 Vendor 页面）。
///
/// 契约与 FlameFeedComponent 同源：
/// - 经 `createTheaterComponentWithOptions:` 创建；未初始化、Content 未 Ready、分类能力缺失、
///   模拟器形态或已存在存活 Theater 组件时返回 nil（fail closed，不猜测降级）。
/// - Host 通过 `rootViewController` 执行 UIKit child containment 并独占持有；
///   MVP 仅支持单 Theater 实例；不承诺与 Feed 组件并存。
/// - `reload` / `switchCategory:` / `searchWithKeyword:`（空/空白关键字拒绝，回调 14202）；
///   上拉或 `loadMore` 分页；item 点击经内部显式路由进入播放页（复用既有 Content 显式
///   Detail 路径）；路由不可用或已有活跃播放会话时回调 14201/14212（fail closed）。
/// - 组件状态与解锁/权益/Login 无关。
@interface FlameTheaterComponent : NSObject

@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) FlameComponentState state;
@property (nonatomic, weak, nullable) id<FlameTheaterComponentListener> listener;
@property (nonatomic, readonly, nullable) UIViewController *rootViewController;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/// 首次加载（分类列表 + 默认分类首页）在组件 attach 后自动发起；Host 可随时再次调用刷新。
- (void)reload;

/// 切换分类（分类名字符串；新分类失败时自动回退上一个分类与列表）。
- (void)switchCategory:(NSString *)categoryId;

/// 基础搜索（单关键字 + 结果分页；复杂搜索能力不在 MVP）。
- (void)searchWithKeyword:(NSString *)keyword;

/// 追加下一页（仅当上一页声明 hasMore；否则本调用为 no-op）。
- (void)loadMore;

@end

@interface FlameContentSdk (TheaterEntry)

/// 创建 Flame Theater 组件（0.3.6 MVP）。
/// 任意线程可调用；SDK 内部归一主线程后同步返回（主线程调用直接执行）。
/// 不可用形态返回 nil（语义同上）；正式运行环境为真机 arm64。
+ (nullable FlameTheaterComponent *)createTheaterComponentWithOptions:(nullable FlameTheaterComponentOptions *)options;

@end

#pragma mark - Component SDK（0.4.5 Feed Stream Lite）

@class FlameFeedStreamComponent;

/// Feed Stream 组件创建参数（Lite）。
@interface FlameFeedStreamComponentOptions : NSObject

/// 解锁流程极简内部 UI（加载覆盖层）开关；默认 YES。关闭仅隐藏展示，流程与事件不变。
@property (nonatomic, assign) BOOL unlockFlowUIEnabled;

@end

/// Feed Stream（Lite）组件事件回调（全部主线程；weak observer）。
/// Lite 形态 = 沉浸式竖向静态封面流 + 点击全屏播放（无内嵌播放）。
@protocol FlameFeedStreamComponentListener <NSObject>
@optional
/// 状态变化（与 FlameFeedComponent 同一状态机契约）。
- (void)feedStreamComponent:(FlameFeedStreamComponent *)component didChangeState:(FlameComponentState)state;
/// 请求级失败（数据失败、handoff fail closed 等；组件保持可用，可重试）。
- (void)feedStreamComponent:(FlameFeedStreamComponent *)component didFailWithCode:(NSString *)code desc:(NSString *)desc;
/// 曝光事件（主导可见 ≥50% 持续 ≥800ms，去重规则见文档；载荷仅 playletId/episode）。
- (void)feedStreamComponent:(FlameFeedStreamComponent *)component didExposeItemWithPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 播放开始事件（点击 → Player Runtime 全屏会话建立成功）。
- (void)feedStreamComponent:(FlameFeedStreamComponent *)component didStartPlaybackForPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 解锁流程脱敏事件（0.4.0 同型；ContentUpdated(unlocked=YES) 只能来自真实 Content State Update）。
- (void)feedStreamComponent:(FlameFeedStreamComponent *)component didReceiveUnlockFlowEvent:(FlameUnlockFlowEvent)event unlocked:(BOOL)unlocked code:(nullable NSString *)code;
@end

/// Flame 自有沉浸式 Feed Stream 组件（Lite；不包装 Vendor 页面、无内嵌播放）。
///
/// 契约与 FlameFeedComponent 同源：factory 主线程同步归一、不可用形态 nil（fail closed）、
/// Host 经 rootViewController 做 UIKit containment、单实例；`reload`/`loadMore` 分页；
/// item 点击经内部显式路由进入全屏播放（Player Runtime 唯一播放入口）；
/// 曝光/播放开始事件脱敏（详见 39 号）；组件状态与解锁/权益/Login 无关。
@interface FlameFeedStreamComponent : NSObject

@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) FlameComponentState state;
@property (nonatomic, weak, nullable) id<FlameFeedStreamComponentListener> listener;
@property (nonatomic, readonly, nullable) UIViewController *rootViewController;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/// 首次 reload 在组件 attach 后自动发起；Host 可随时再次调用刷新。
- (void)reload;

/// 追加下一页（仅当上一页声明 hasMore；否则本调用为 no-op）。
- (void)loadMore;

@end

@interface FlameContentSdk (FeedStreamEntry)

/// 创建 Flame Feed Stream 组件（0.4.5 Lite）。
/// 任意线程可调用；SDK 内部归一主线程后同步返回（主线程调用直接执行）。
/// 不可用形态返回 nil（语义同上）；正式运行环境为真机 arm64。
+ (nullable FlameFeedStreamComponent *)createFeedStreamComponentWithOptions:(nullable FlameFeedStreamComponentOptions *)options;

@end

#pragma mark - Component SDK（0.4.10 Feed Stream Container / Vendor 沉浸形态）

@class FlameFeedStreamContainerComponent;

/// Feed Stream Container 组件创建参数（Vendor 沉浸形态）。
@interface FlameFeedStreamContainerComponentOptions : NSObject

/// 起始短剧（可选；<= 0 = 平台默认推荐流）。
@property (nonatomic, assign) NSInteger initialPlayletId;

@end

/// Feed Stream Container 组件事件回调（全部主线程；weak observer）。
/// Container 形态 = 官方沉浸式滑滑流（自动播放 / 上下滑切换 / 点击进短剧详情）。
///
/// 事件口径与 Lite 并存不混用（0.4.8 冻结）：
/// - didChangeItem = "切换即曝光"（切换即触发，含首入；不同于 Lite 的主导可见 ≥800ms 口径）；
/// - didStartPlayback = "当前 item 视频开始播放"（不同于 Lite 的"全屏会话建立"语义）；
/// - 点击进详情为页面内导航，不经 Flame 全屏播放组件，无对应 handoff 事件。
@protocol FlameFeedStreamContainerComponentListener <NSObject>
@optional
/// 状态变化（与组件家族同一状态机契约）。
- (void)feedStreamContainerComponent:(FlameFeedStreamContainerComponent *)component
                        didChangeState:(FlameComponentState)state;
/// 请求级失败（数据请求/刷新/播放错误；组件保持可用，可由用户在页面内重试）。
- (void)feedStreamContainerComponent:(FlameFeedStreamContainerComponent *)component
                         didFailWithCode:(NSString *)code desc:(NSString *)desc;
/// item 切换事件（切换即曝光口径；载荷仅 playletId/episode）。
- (void)feedStreamContainerComponent:(FlameFeedStreamContainerComponent *)component
            didChangeItemWithPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 播放开始事件（当前 item 视频真实开始播放）。
- (void)feedStreamContainerComponent:(FlameFeedStreamContainerComponent *)component
     didStartPlaybackForPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
/// 解锁流程脱敏事件（0.4.0 同型；注意：详情内解锁由平台链路裁决，本组件零权益参与）。
- (void)feedStreamContainerComponent:(FlameFeedStreamContainerComponent *)component
    didReceiveUnlockFlowEvent:(FlameUnlockFlowEvent)event unlocked:(BOOL)unlocked code:(nullable NSString *)code;
@end

/// Flame Feed Stream Container 组件（0.4.10；官方沉浸式滑滑流形态）。
///
/// 契约与组件家族同源：factory 主线程同步归一、不可用形态 nil（fail closed）、
/// Host 经 rootViewController 做 UIKit containment、与 Feed Stream（Lite）单表面互斥
/// （同一时间至多一个 Stream 形态组件存活，且至多一个官方滑滑流实例存在）。
/// 内容数据与埋点由官方页面自管（无自建聚合页推荐降质风险）；组件状态与解锁/权益/Login 无关。
@interface FlameFeedStreamContainerComponent : NSObject

@property (nonatomic, copy, readonly) NSString *componentId;
@property (nonatomic, readonly) FlameComponentState state;
@property (nonatomic, weak, nullable) id<FlameFeedStreamContainerComponentListener> listener;
@property (nonatomic, readonly, nullable) UIViewController *rootViewController;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

@interface FlameContentSdk (FeedStreamContainerEntry)

/// 创建 Flame Feed Stream Container 组件（0.4.10 官方沉浸形态）。
/// 任意线程可调用；SDK 内部归一主线程后同步返回（主线程调用直接执行）。
/// 不可用形态返回 nil（未初始化 / Content 未 Ready / 模拟器 / 已有存活 Stream 组件
/// （Lite 或 Container）或官方滑滑流容器占用时均属预期，fail closed）。
+ (nullable FlameFeedStreamContainerComponent *)createFeedStreamContainerComponentWithOptions:
        (nullable FlameFeedStreamContainerComponentOptions *)options
                                                                                   listener:
        (nullable id<FlameFeedStreamContainerComponentListener>)listener;

@end

NS_ASSUME_NONNULL_END
