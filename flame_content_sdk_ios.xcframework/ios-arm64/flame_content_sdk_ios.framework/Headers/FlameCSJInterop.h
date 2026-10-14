#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class DJXPlayletConfig;
@class DJXPlayletInfoModel;

/// CSJ Native Interop 错误域（FlameCSJInterop.prepareUnlockPlayletConfig: 的 NSError）。
extern NSString * const FlameCSJInteropErrorDomain;

typedef NS_ERROR_ENUM(FlameCSJInteropErrorDomain, FlameCSJInteropErrorCode) {
    /// config 为 nil。
    FlameCSJInteropErrorInvalidConfig = 1,
    /// Flame 未 READY / CSJ Content 未启动 / 当前形态不支持（Ads-only、模拟器）。
    FlameCSJInteropErrorNotReady = 2,
};

/// Flame × CSJ Native Interop（迁移兼容模式；显式 import 启用，不进主 umbrella）。
///
/// 产品边界（冻结，见 docs/context/csj_native_interop/11 号）：
/// - Flame 独占：初始化 / 隐私 / Direct GroMore / Custom Unlock / Reward→Content 桥 /
///   解锁裁决（Reward Success != Unlock Success）；
/// - 客户 Native 直用：DJXPlayletManager 数据/搜索/分类/收藏/点赞/历史、页面创建、
///   DrawVideo/Detail、播放控制、倍速、seek、选集，以及 playerDelegate /
///   config.delegate / customViewDelegate 等客户自有槽位（Flame 永不触碰）；
/// - 本头是 Core Public API 的唯一三方类型例外面：调用方需自行
///   `#import <PangrowthDJX/...>`（或 Swift `import PangrowthDJX`）获取具体类型。
///
/// 使用模型（每页一个已 prepare 的 config）：
/// 1. Flame READY 后，客户创建 `DJXPlayletConfig` 并填写业务参数；
/// 2. 调用 `prepareUnlockPlayletConfig:onEnterDetail:error:` 原子完成 Flame 解锁配置
///    （Specific 模式 + per-config 解锁委托 + 当前全局免费/解锁集数）；
/// 3. 用该 config 创建 PangrowthDJX 原生页面（页面创建在 prepare 之后）；
/// 4. 从流进入详情：新建一个 `DJXPlayletConfig` → 再次 prepare → 再创建原生详情页；
///    push/present 由宿主负责（vendor 的 enterPlayPage/clickEnterView 会回调
///    onEnterDetail 携带当前剧 InfoModel）。
///
/// Unsupported / outside Flame-managed unlock contract（不做技术拦截，后果自担）：
/// 客户将 config 改回 Common 模式、替换 Flame 注入的 interfaceDelegate、自行回传
/// Reward 成功、同一次解锁另起一套广告、或对未 prepare 的页面要求 Flame 托管解锁。
///
/// 一次性语义：与 Flame 初始化一致，底层三方栈进程级一次；本 API 不改变该语义。
@interface FlameCSJInterop : NSObject

/// Native Interop 可用性。
/// YES = Flame 已 READY 且 CSJ Content provider 已成功启动（非 Ads-only、非模拟器形态）。
/// NO 时调用 prepare 必失败（FlameCSJInteropErrorNotReady）。
@property (nonatomic, class, readonly) BOOL csjNativeInteropReady;

/// 将客户创建的 `DJXPlayletConfig` 原子注册为 Flame-managed unlock config：
/// - `playletUnlockADMode = Specific`（开发者自定义广告 = Flame Reward 承接）；
/// - `interfaceDelegate` = 本 config 专属的 Flame 解锁委托（per-config，弱槽位由
///   与 config 绑定的适配器对象满足，config 存活期内不释放）；
/// - 免费集数/单次解锁集数 = Flame 当前全局解锁策略（`setContentUnlockConfig` 为权威，
///   本 API 不另设参数）；
/// - onEnterDetail：vendor「进入详情页」时机（clickEnterView）转发，携带当前剧
///   InfoModel；宿主据此新建 config → 再次 prepare → 创建原生详情页（宿主导航）。
///
/// 幂等：对同一 config 重复 prepare 会更新 onEnterDetail 并重放注入，返回 YES。
/// 返回 NO（fail closed）：config 为 nil（InvalidConfig）或未 READY（NotReady）；
/// 失败的 config 不得用于 Flame 托管解锁页面。
+ (BOOL)prepareUnlockPlayletConfig:(DJXPlayletConfig *)config
                      onEnterDetail:(nullable void (^)(DJXPlayletInfoModel *infoModel))onEnterDetail
                              error:(NSError * _Nullable * _Nullable)error;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
