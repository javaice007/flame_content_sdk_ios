#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <flame_content_sdk_ios/FlameContentSdk.h>

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

@end

NS_ASSUME_NONNULL_END
