//
//  WebSocket.h
//  WebSocket
//
//  Created by YourtionGuo on 11/01/2017.
//  Copyright © 2017 Yourtion. All rights reserved.
//

#import <UIKit/UIKit.h>

//! Project version number for WebSocket.
FOUNDATION_EXPORT double WebSocketVersionNumber;

//! Project version string for WebSocket.
FOUNDATION_EXPORT const unsigned char WebSocketVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <WebSocket/PublicHeader.h>
#define kWebSocketDefaultPort        80
#define kWebSocketDefaultSecurePort 443

#define kWebSocketScheme       "ws"
#define kWebSocketSecureScheme "wss"

#include <CoreFoundation/CoreFoundation.h>

#include "WebSocketTypes.h"
#include "WebSocketCore.h"
#include "WebSocketClient.h"
#include "cuEnc64.h"

#endif

