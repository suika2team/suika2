/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#import "GameView.h"
#import "GameViewControllerProtocol.h"

#import <AVFoundation/AVFoundation.h>

#import "opennovel.h"

@implementation GameView

//
// Declare that we use a video player layer.
//
+ (Class)layerClass {
    return AVPlayerLayer.class;
}

//
// Get a GameViewController from an event.
//
- (id<GameViewControllerProtocol>) viewControllerFrom:(NSEvent *)event {
    NSObject *viewController = event.window.contentViewController;
    if ([viewController conformsToProtocol:@protocol(GameViewControllerProtocol)])
        return (NSObject<GameViewControllerProtocol> *)viewController;
    return nil;
}

//
// Called when a mouse button is pressed.
//
- (void)mouseDown:(NSEvent *)event {
    id<GameViewControllerProtocol> viewController = [self viewControllerFrom:event];
    NSPoint point = [viewController windowPointToScreenPoint:[event locationInWindow]];
    on_event_mouse_press(MOUSE_LEFT, point.x, point.y);
}

//
// Called when a mouse button is released.
//
- (void)mouseUp:(NSEvent *)event {
    id<GameViewControllerProtocol> viewController = [self viewControllerFrom:event];
    NSPoint point = [viewController windowPointToScreenPoint:[event locationInWindow]];
    on_event_mouse_release(MOUSE_LEFT, point.x, point.y);
}

//
// Called when a right mouse button is pressed.
//
- (void)rightMouseDown:(NSEvent *)event {
    id<GameViewControllerProtocol> viewController = [self viewControllerFrom:event];
    NSPoint point = [viewController windowPointToScreenPoint:[event locationInWindow]];
    on_event_mouse_press(MOUSE_RIGHT, point.x, point.y);
}

//
// Called when a right mouse button is released.
//
- (void)rightMouseUp:(NSEvent *)event {
    id<GameViewControllerProtocol> viewController = [self viewControllerFrom:event];
    NSPoint point = [viewController windowPointToScreenPoint:[event locationInWindow]];
    on_event_mouse_release(MOUSE_RIGHT, point.x, point.y);
}

//
// Called when a mouse is dragged.
//
- (void)mouseDragged:(NSEvent *)event {
    id<GameViewControllerProtocol> viewController = [self viewControllerFrom:event];
    NSPoint point = [viewController windowPointToScreenPoint:[event locationInWindow]];
    on_event_mouse_move(point.x, point.y);
}

//
// Called when a mouse wheel is pressed.
//
- (void)scrollWheel:(NSEvent *)event {
    int delta = [event deltaY];
    if (delta > 0) {
        on_event_key_press(KEY_UP);
        on_event_key_release(KEY_UP);
    } else if (delta < 0) {
        on_event_key_press(KEY_DOWN);
        on_event_key_release(KEY_DOWN);
    }
}

@end
