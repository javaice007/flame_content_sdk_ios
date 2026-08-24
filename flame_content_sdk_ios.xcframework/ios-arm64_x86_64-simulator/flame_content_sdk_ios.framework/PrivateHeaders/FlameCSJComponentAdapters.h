#import <Foundation/Foundation.h>
#import "FlameComponentAdapters.h"

NS_ASSUME_NONNULL_BEGIN

/// CSJ（PangrowthDJX）内容数据 Adapter：Feed 列表数据来源为官方推荐流分页接口。
/// 模拟器 / 未启用 Content（FLAME_CONTENT_DEVICE 关闭）时 capability 为 NO，
/// 数据请求按 14120/14000 语义失败（fail closed，零三方符号）。
@interface FlameCSJContentDataAdapter : NSObject <FlameContentDataAdapter>
@end

/// CSJ 播放 Adapter：Feed → Player 显式路径，复用既有
/// FlameCSJContentRuntime playletDetailViewControllerForSkitId:episode: 创建路径
/// （Flame 持有 Config、Custom Unlock 挂接保持既有语义，不修改现有 Runtime）。
@interface FlameCSJPlayerAdapter : NSObject <FlamePlayerAdapter>
@end

/// CSJ Binding Adapter：当前 PangrowthDJX 2.9.0.6 未提供官方 instance binding /
/// Detail correlation / 生命周期关联，全部能力位恒 NO（fail closed，无启发式）。
@interface FlameCSJBindingAdapter : NSObject <FlameBindingAdapter>
@end

/// CSJ Vendor Stream Container Adapter（0.4.10）：以官方 DJXDrawVideoViewController
/// `customAppear` + view 嵌入形态承载（46 号；0.2.0 容器模式复用）。
/// 模拟器 / 未启用 Content 时 capability 为 NO、创建返回 nil（fail closed，零三方符号）。
@interface FlameCSJFeedStreamVendorAdapter : NSObject <FlameFeedStreamVendorAdapter>
@end

NS_ASSUME_NONNULL_END
