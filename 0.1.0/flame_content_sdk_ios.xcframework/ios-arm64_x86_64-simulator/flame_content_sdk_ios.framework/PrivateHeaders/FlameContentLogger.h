#import <Foundation/Foundation.h>

@interface FlameContentLogger : NSObject
+ (void)setDebugEnabled:(BOOL)enabled;
+ (BOOL)isDebugEnabled;
+ (void)debug:(NSString *)message;
+ (void)error:(NSString *)message;
@end
