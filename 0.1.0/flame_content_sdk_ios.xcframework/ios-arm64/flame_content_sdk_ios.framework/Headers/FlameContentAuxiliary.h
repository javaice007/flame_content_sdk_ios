#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameContentSdk.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark - Flame Content Playlet Model（公开只读数据模型，零三方类型暴露）

/// Flame 短剧信息模型：仅由 SDK 返回，Host 只读（不可自行构造）。
/// 所有属性 readonly；init/new 不可用。
@interface FlameContentPlaylet : NSObject

@property (nonatomic, assign, readonly) NSInteger playletId;        // 短剧业务 ID（同 skitId）
@property (nonatomic, copy, readonly, nullable) NSString *title;    // 短剧名
@property (nonatomic, copy, readonly, nullable) NSString *coverURL; // 封面图 URL
@property (nonatomic, assign, readonly) NSInteger totalEpisodes;    // 总集数
@property (nonatomic, assign, readonly) NSInteger currentEpisode;   // 当前播放到第几集（继续观看用）
@property (nonatomic, assign, readonly) NSInteger status;           // 1=未完结
@property (nonatomic, copy, readonly, nullable) NSString *categoryName; // 分类名
@property (nonatomic, copy, readonly, nullable) NSString *playletDescription; // 简介
@property (nonatomic, assign, readonly) BOOL favorite;              // 是否已收藏

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

#pragma mark - Content Unlock Policy（公开解锁策略配置）

@interface FlameContentSdk (ContentUnlockPolicy)

/// 配置内容解锁策略：前 N 集免费观看 + 每次看广告解锁 M 集。
/// 默认（不调用此方法）：免费 10 集、每次解锁 5 集。
/// 广告位不由 Host 配置——由 Flame 平台 /sdk/init 的 scenePIds.content_unlock 动态下发。
///
/// @param freeEpisodeCount   免费观看集数（>= 0；0 表示从第 1 集即需解锁）
/// @param unlockEpisodeCount 每次看广告解锁集数（> 0）
/// @return YES=配置成功；NO=参数非法（保持上一次有效配置或默认值）
///
/// 线程安全；仅影响之后创建的内容页面，已打开页面不热更新。
+ (BOOL)setContentUnlockConfigWithFreeEpisodeCount:(NSInteger)freeEpisodeCount
                                unlockEpisodeCount:(NSInteger)unlockEpisodeCount NS_SWIFT_NAME(setContentUnlockConfig(freeEpisodeCount:unlockEpisodeCount:));

/// 当前生效的免费集数（默认 10）
+ (NSInteger)currentFreeEpisodeCount;

/// 当前生效的每次解锁集数（默认 5）
+ (NSInteger)currentUnlockEpisodeCount;

@end

#pragma mark - Content Auxiliary APIs（P0 辅助能力）

/// 列表回调（主线程；playlets 非空数组时 success，否则 failure）。
/// hasMore 仅在 PAGED 接口中有意义（当前所有列表均为 ONE_SHOT / 固定首页，hasMore 恒 NO）。
typedef void (^FlamePlayletListHandler)(NSArray<FlameContentPlaylet *> *playlets, BOOL hasMore);
/// 单个短剧回调（主线程）。
typedef void (^FlamePlayletHandler)(FlameContentPlaylet *playlet);
/// 无返回值回调（主线程）。
typedef void (^FlameContentVoidHandler)(void);
/// 错误回调（主线程）。code 为 Flame 字符串错误码（与 FlameCallback.fail:desc: 一致风格）。
typedef void (^FlameContentErrorHandler)(NSString *code, NSString *desc);

@interface FlameContentSdk (ContentAuxiliary)

/// 搜索短剧（关键字模糊搜索）。回调主线程。
/// 语义：ONE_SHOT（provider 当前不支持可靠分页，首次调用返回当前可用结果）。
/// keyword 不得为空或纯空白。
+ (void)searchPlayletsWithKeyword:(NSString *)keyword
                          success:(FlamePlayletListHandler _Nullable)success
                          failure:(FlameContentErrorHandler _Nullable)failure;

/// 继续观看列表（观看历史）。回调主线程。
/// 语义：返回最近观看的短剧（最多 50 条；provider 不支持 loadMore，hasMore 恒 NO）。
+ (void)requestContinueWatchingWithSuccess:(FlamePlayletListHandler _Nullable)success
                                   failure:(FlameContentErrorHandler _Nullable)failure;

/// 收藏列表。回调主线程。
/// 语义：返回当前收藏（最多 50 条；provider 支持 hasMore 但 Flame 当前不暴露 loadMore，hasMore 恒 NO）。
+ (void)requestFavoritePlayletsWithSuccess:(FlamePlayletListHandler _Nullable)success
                                   failure:(FlameContentErrorHandler _Nullable)failure;

/// 收藏指定短剧（playletId > 0）。回调主线程。
+ (void)favoritePlayletWithPlayletId:(NSInteger)playletId
                             success:(FlameContentVoidHandler _Nullable)success
                             failure:(FlameContentErrorHandler _Nullable)failure;

/// 取消收藏指定短剧（playletId > 0）。回调主线程。
+ (void)unfavoritePlayletWithPlayletId:(NSInteger)playletId
                               success:(FlameContentVoidHandler _Nullable)success
                               failure:(FlameContentErrorHandler _Nullable)failure;

/// 按 ID 查询短剧信息（用于 Deep Link / 收藏/历史恢复前确认物料仍存在）。回调主线程。
+ (void)requestPlayletWithPlayletId:(NSInteger)playletId
                            success:(FlamePlayletHandler _Nullable)success
                            failure:(FlameContentErrorHandler _Nullable)failure;

/// 清空观看历史（⚠️ 破坏性操作，仅 Host 明确用户意图时调用）。回调主线程。
+ (void)clearWatchHistoryWithSuccess:(FlameContentVoidHandler _Nullable)success
                             failure:(FlameContentErrorHandler _Nullable)failure;

@end

#pragma mark - Content User / Login（A5.2 登录能力）

/// Flame Content 用户登录状态。
typedef NS_ENUM(NSInteger, FlameContentUserState) {
    FlameContentUserStateAnonymous = 0,  // 未登录（游客模式，记录与设备绑定）
    FlameContentUserStateLoggedIn  = 1,  // 已登录（记录与 uid 绑定）
};

/// Flame Content 登录回调（主线程）。
typedef void (^FlameContentLoginHandler)(BOOL success, NSString *_Nullable desc);

@interface FlameContentSdk (ContentLogin)

/// 内容登录：使用 Host Backend 生成的官方签名 paramsString。
/// paramsString 格式（官方要求）：`nonce=xxx&ouid=xxx&timestamp=xxx&sign=xxx`
/// ⚠️ server key 必须保留在 Host Backend，不得传入客户端。
///
/// 登录后：观看记录、收藏、解锁记录与 uid 绑定（跨启动持久化，设备无关）。
/// 回调主线程。
+ (void)contentLoginWithParamsString:(NSString *)paramsString
                          completion:(FlameContentLoginHandler _Nullable)completion;

/// 内容退出登录：清除 uid 绑定，回退到游客模式（设备绑定）。
/// 回调主线程。
+ (void)contentLogoutWithCompletion:(FlameContentLoginHandler _Nullable)completion;

/// 当前内容登录状态。
+ (FlameContentUserState)contentUserState;

/// 当前是否已登录。
+ (BOOL)isContentLoggedIn;

@end

NS_ASSUME_NONNULL_END
