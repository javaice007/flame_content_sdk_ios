#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Flame 隐私开关集合（0.2.1；字段名与穿山甲官方选择子对齐）。
///
/// 契约：
/// - 经 `+initWithAppId:appKey:privacyOptions:callback:` 在初始化时传入；
///   不传（nil）= 全部保持现状默认行为。
/// - 三方栈（GroMore 广告 / CSJ Content）进程内仅初始化一次：options 只在
///   首次真实初始化时生效；SDK 已 Initialized 后再传入会被忽略（仅 debug 日志），
///   `clear` 后重试也不会改变已启动的三方栈行为。
/// - Host 可在任意线程构建本对象；Flame 在 init 入口快照字段值，
///   之后 Host 对本对象的修改不影响当次初始化。
@interface FlamePrivacyOptions : NSObject <NSCopying>

/// 位置信息开关（默认 YES）。
/// 广告栈：BUAdSDKPrivacyProvider.canUseLocation。
@property (nonatomic, assign) BOOL canUseLocation;

/// 设备标识开关（默认 YES；IDFA 等）。
/// 广告栈：mediation.forbiddenIDFA / allowUploadDeviceInfo；内容栈：allowAccessIDFA。
@property (nonatomic, assign) BOOL allowAccessIDFA;

/// 网络信息开关（默认 YES；Wi-Fi BSSID）。
/// 广告栈：BUAdSDKPrivacyProvider.canUseWiFiBSSID。
@property (nonatomic, assign) BOOL canUseWiFiBSSID;

/// 青少年模式（默认 NO）。
/// 广告栈：BUAdSDKConfiguration.ageGroup = Teenager；内容栈：turnOnTeenMode。
/// 注意穿山甲官方行为：青少年模式开启后内容栈可能不返回短剧内容。
@property (nonatomic, assign) BOOL turnOnTeenMode;

/// 备案号筛选（默认 NO；仅内容栈生效）。
/// YES = 聚合页/滑滑流仅展示有备案号的短剧内容（DJXAuthorityConfigDelegate.isOnlyICPNumber）。
@property (nonatomic, assign) BOOL isOnlyICPNumber;

@end

NS_ASSUME_NONNULL_END
