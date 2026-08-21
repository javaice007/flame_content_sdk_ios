#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// /api/v1/sdk/init 网络客户端。
/// - NSURLSession POST JSON，默认 15s 超时，无重试
/// - completion 一律主线程回调，block 显式 copy
/// - 实例可 cancel（clear/新请求前取消旧任务）
/// - session 可注入（单测 seam）
@interface FlameNetworkUtils : NSObject

- (instancetype)initWithSession:(NSURLSession *)session NS_DESIGNATED_INITIALIZER;
- (instancetype)init;  // sharedSession

/// 发起 POST JSON 请求。completion 参数：response 为服务端返回 JSON 根对象（校验为 dictionary），
/// networkError 为传输层错误（已映射 Flame 错误码/desc），两者互斥。
- (void)postJSONObject:(NSDictionary *)body
                toURL:(NSURL *)url
               timeout:(NSTimeInterval)timeout
            completion:(void (^)(NSDictionary *_Nullable response, NSString *_Nullable errorCode, NSString *_Nullable errorDesc))completion;

- (void)cancel;

@end

NS_ASSUME_NONNULL_END
