#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "../Public/FlameContentAuxiliary.h"

NS_ASSUME_NONNULL_BEGIN

/// Component SDK 内部错误码（142xx 分段；0.3.0 设计 §2 错误码方向）。
/// 仅用于组件层日志与 listener 回调；不改变既有 140xx/141xx 语义。
extern NSString *const FlameComponentErrorCodeCapability;   ///< 14200 能力缺失（内部诊断用）
extern NSString *const FlameComponentErrorCodeRoute;        ///< 14201 路由不可用（fail closed）
extern NSString *const FlameComponentErrorCodeInvalid;      ///< 14202 非法组件请求参数
extern NSString *const FlameComponentErrorCodeData;         ///< 14210 组件数据请求失败
extern NSString *const FlameComponentErrorCodePlayerPresent;///< 14211 Player 呈现失败
extern NSString *const FlameComponentErrorCodePlayerBusy;   ///< 14212 已有活跃播放会话（fail closed）

/// Component SDK 内部分页数据契约（未公开）。
/// 与公开 Auxiliary API 的 ONE_SHOT 语义相互独立：本契约显式携带 hasMore/nextCursor。
@interface FlameContentPage : NSObject

@property (nonatomic, copy, readonly) NSArray<FlameContentPlaylet *> *items;
@property (nonatomic, readonly) BOOL hasMore;
@property (nonatomic, copy, readonly, nullable) NSString *nextCursor;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/// 仅供 SDK 内部与测试构造。
+ (instancetype)flame_pageWithItems:(NSArray<FlameContentPlaylet *> *)items
                           hasMore:(BOOL)hasMore
                         nextCursor:(nullable NSString *)nextCursor;

@end

/// 内容数据 Adapter：Feed 列表数据唯一来源（Vendor 隔离边界；内部协议，不公开）。
/// 实现方保证 completion 主线程回调；不可用时 capability 为 NO（fail closed）。
@protocol FlameContentDataAdapter <NSObject>

@property (nonatomic, readonly) BOOL contentDataCapabilityAvailable;

/// 拉取一页 Feed 数据。cursor 为 nil 表示首页；否则为上一次返回的 nextCursor。
- (void)requestFeedPageWithCursor:(nullable NSString *)cursor
                       completion:(void (^)(FlameContentPage *_Nullable page,
                                            NSString *_Nullable code,
                                            NSString *_Nullable desc))completion;

@optional

#pragma mark - Theater 能力（0.3.6；@optional 保证既有实现零破坏）

/// Theater 分类能力位。未实现本组方法的 Adapter 视为无 Theater 能力（fail closed）。
@property (nonatomic, readonly) BOOL categoryCapabilityAvailable;

/// 分类列表（分类名字符串数组；15 号文档 T2 冻结模型）。
- (void)requestCategoryListWithCompletion:(void (^)(NSArray<NSString *> *_Nullable categories,
                                                    NSString *_Nullable code,
                                                    NSString *_Nullable desc))completion;

/// 分类下剧集分页（cursor = nil 表示第 1 页；nextCursor 为十进制页码字符串）。
- (void)requestCategoryPlayletPageWithCategory:(NSString *)categoryId
                                        cursor:(nullable NSString *)cursor
                                    completion:(void (^)(FlameContentPage *_Nullable page,
                                                         NSString *_Nullable code,
                                                         NSString *_Nullable desc))completion;

/// 搜索分页（vendor 回调 hasMore 直接映射）。
- (void)searchPlayletPageWithKeyword:(NSString *)keyword
                              cursor:(nullable NSString *)cursor
                          completion:(void (^)(FlameContentPage *_Nullable page,
                                               NSString *_Nullable code,
                                               NSString *_Nullable desc))completion;

/// Stream 曝光回传（0.4.5；@optional）：CSJ 实现内部映射官方 trackEvent
/// （自建流 = 官方"自建聚合页"场景，ClientShow 等事件）。未实现则跳过（公开面零新增）。
- (void)trackExposureForPlaylet:(FlameContentPlaylet *)playlet;

@end

/// 播放 Adapter：仅满足 Feed → Player 显式路径（playletId + episode 已知）。
/// 0.3.2 MVP 不提供播放控制面、小窗、多实例；Vendor Player 类型不外泄。
@protocol FlamePlayerAdapter <NSObject>

@property (nonatomic, readonly) BOOL playerCapabilityAvailable;

/// 从 presenter 全屏呈现指定短剧的播放页（复用既有显式 Detail 创建路径）。
- (void)presentDetailForPlayletId:(NSInteger)playletId
                          episode:(NSInteger)episode
              fromViewController:(UIViewController *)presenter
                       completion:(void (^)(NSString *_Nullable code,
                                            NSString *_Nullable desc))completion;

@end

/// Binding Adapter：能力位全部依赖 Vendor 官方提供的 instance binding /
/// Detail correlation / 生命周期关联。任一能力缺失时必须保持 NO（fail closed），
/// 不得以启发式补位。
@protocol FlameBindingAdapter <NSObject>

@property (nonatomic, readonly) BOOL instanceBindingCapabilityAvailable;
@property (nonatomic, readonly) BOOL detailCorrelationCapabilityAvailable;
@property (nonatomic, readonly) BOOL lifecycleCorrelationCapabilityAvailable;

@end

#pragma mark - Vendor Stream Container Adapter（0.4.10；46–49 号冻结）

/// 声明于 content/csj/FlameContentContainerViewController+Internal.h（0.2.0 容器壳生命周期协议）。
@protocol FlameContentContainerFeedLifecycle;

/// Vendor Stream 事件出口（Vendor 类型零泄漏；实现方保证主线程回调或经组件归一）。
/// 口径（48 号 §3，与 Lite 并存不混用）：
/// - didChangeItem = "切换即曝光"（Vendor currentVideoChanged 语义，首入也触发）；
/// - didStartPlayback = "Vendor startPlay 事件"（非 Lite 的"会话建立无同步失败"语义）；
/// - didCompletePlayback = 内部吸收（连播/播完，不发公开事件，48 号冻结）。
@protocol FlameFeedStreamVendorEventSink <NSObject>

- (void)vendorStreamDidChangeItemWithPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
- (void)vendorStreamDidStartPlaybackWithPlayletId:(NSInteger)playletId episode:(NSInteger)episode;
- (void)vendorStreamDidFailWithCode:(NSString *)code desc:(nullable NSString *)desc;
- (void)vendorStreamDidCompletePlaybackWithPlayletId:(NSInteger)playletId episode:(NSInteger)episode;

@end

/// Vendor Stream Container 初始 viewport（内部值对象；不向 Host 暴露 Vendor 几何）。
@interface FlameFeedStreamViewport : NSObject

@property (nonatomic, readonly) CGSize size;
@property (nonatomic, readonly) CGFloat topInset;
@property (nonatomic, readonly) CGFloat bottomInset;

- (instancetype)initWithSize:(CGSize)size
                     topInset:(CGFloat)topInset
                  bottomInset:(CGFloat)bottomInset NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

/// Vendor Stream Container Adapter（内部协议，不公开；49 号 Q3 五职责白名单）：
/// 创建 Vendor Stream / 生命周期桥接 / 事件转换 / Unlock Flow 接线 / Flame 状态同步。
/// 禁止：权益判断、Reward 修改、Vendor 类型泄露、自研播放器。
@protocol FlameFeedStreamVendorAdapter <NSObject>

/// 能力位：真机 + Content Ready = YES；模拟器 / 未初始化 = NO（fail closed，14120/14000 语义）。
@property (nonatomic, readonly) BOOL vendorStreamCapabilityAvailable;

/// 创建 Vendor Stream 页面（customAppear 形态；内部完成 config 组装与事件桥接挂接）。
/// topSkitId <= 0 = Vendor 默认推荐流；sink 为事件出口（adapter 侧弱持有，组件侧保活桥接对象）。
/// 返回值已遵从 FlameContentContainerFeedLifecycle（由 0.2.0 容器壳驱动 appear/disappear）。
- (nullable UIViewController<FlameContentContainerFeedLifecycle> *)createVendorStreamViewControllerWithTopSkitId:(NSInteger)topSkitId
                                                                                                      viewport:(FlameFeedStreamViewport *)viewport
                                                                                                     eventSink:(nullable id<FlameFeedStreamVendorEventSink>)eventSink;

@end

NS_ASSUME_NONNULL_END
