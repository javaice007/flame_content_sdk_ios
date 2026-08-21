#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// AES-GCM 加解密（与旧 Flame iOS SDK 的 edata 协议兼容：Base64(IV(12B) + Ciphertext + TAG(16B))，
/// key = appKey 的 UTF-8 原始字节，按长度自动选择 128/192/256）。
///
/// 实现说明：不引入 OpenSSL 依赖，使用 CommonCrypto 的 AES-ECB（派生 H）与 CTR 模式（密钥流）
/// 加上自实现 GHASH（NIST SP 800-38D），正确性由 NIST 官方测试向量与单测覆盖。
@interface FlameAesGcmUtils : NSObject

/// GCM 解密并校验 TAG；TAG 不匹配或参数非法返回 nil。
/// ciphertext/tag 长度可变（tag 支持 16B），iv 支持 12B（协议固定 12B）。
+ (nullable NSData *)decryptAESGCM:(NSData *)ciphertext
                               key:(NSData *)key
                               iv:(NSData *)iv
                              tag:(NSData *)tag;

/// GCM 加密（测试与 fixture 构造用；与 decrypt 对偶）。
+ (nullable NSData *)encryptAESGCM:(NSData *)plaintext
                               key:(NSData *)key
                               iv:(NSData *)iv
                              tag:(NSData * _Nullable * _Nullable)tagOut;

/// edata = Base64(IV(12) || CT || TAG(16)) 一步解密，返回明文 JSON 数据；失败返回 nil。
+ (nullable NSData *)decryptEdataBase64:(NSString *)edata appKey:(NSString *)appKey;

@end

NS_ASSUME_NONNULL_END
