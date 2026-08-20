#import "../provider/FlameAdsProvider.h"

NS_ASSUME_NONNULL_BEGIN

/// Direct GroMore Provider（内部实现，不暴露给 Host）。
/// - 初始化：[BUAdSDKConfiguration configuration]（共享单例，runtime 实测确认）+ useMediation=YES
///   → +[BUAdSDKManager startWithAsyncCompletionHandler:]（回调非主线程，本类归一主线程后回调）
/// - GroMore 进程内仅支持初始化一次；本类对重复 initialize 返回既有状态。
@interface FlameGroMoreProvider : NSObject <FlameAdsProvider>

@end

NS_ASSUME_NONNULL_END
