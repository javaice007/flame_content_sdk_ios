#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Flame 后端 /api/v1/sdk/init 请求签名（与旧 Flame iOS SDK 协议兼容）。
/// 规则：业务参数按 key 字典序升序拼 k1=v1&k2=v2，再追加 &key=<appKey>，取 MD5 小写 hex。
@interface FlameMd5SignatureUtils : NSObject

/// 参数 value 均按字符串处理；params 为空时仅签 key。返回小写 hex（32 位）。
+ (nullable NSString *)signWithParams:(NSDictionary<NSString *, NSString *> *)params appKey:(NSString *)appKey;

@end

NS_ASSUME_NONNULL_END
