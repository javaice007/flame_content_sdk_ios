#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// 内部内容能力 Provider 抽象（A3）。
/// 职责：初始化/启动、ready 状态、clear 复位；Core 不直接依赖 DJX 具体类。
/// completion 一律主线程回调，code/desc 为 Flame 错误体系字符串码。
@protocol FlameContentProvider <NSObject>

/// 是否已完成启动（进程内状态）。
@property (nonatomic, assign, readonly) BOOL isReady;

/// 启动内容能力。重复调用：已 Ready 时直接成功回调；进行中调用按并发合并处理。
- (void)startWithCompletion:(void (^)(BOOL success, NSString *_Nullable code, NSString *_Nullable desc))completion;

/// 复位 Flame 侧状态（底层 SDK 若仅支持进程级一次初始化，则只复位本层标志）。
- (void)reset;

@end

NS_ASSUME_NONNULL_END
