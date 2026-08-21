#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/// ⚠️ Debug/InternalTesting 专用渲染探针（A4D 专项调研；非 Public API）。
/// 递归扫描当前最上层页面 View Tree，仅输出结构信息（class/frame/alpha/背景色/layer/zPosition/
/// Metal 层参数），用于「粉/黑遮罩 vs 播放器渲染」分型。不输出用户数据/URL/凭证/配置内容；
/// 不调用任何私有 selector、不修改三方状态。
@interface FlameContentRenderProbe : NSObject

/// 扫描当前 keyWindow 最上层 presented VC 的视图树（主线程调用）。
/// tag 会打进每行前缀，便于多时间点对照（如 page+3s / page+8s）。
+ (void)dumpTopViewHierarchyWithTag:(NSString *)tag;

@end

NS_ASSUME_NONNULL_END
