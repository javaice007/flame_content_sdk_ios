#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol FlameCallback <NSObject>
- (void)success;
- (void)fail:(NSString *)code desc:(NSString *)desc;
@end

NS_ASSUME_NONNULL_END
