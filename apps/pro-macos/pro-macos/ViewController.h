// -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

@import AppKit;
@import AVFoundation;

#import "GameViewControllerProtocol.h"

@interface ViewController : NSViewController <NSApplicationDelegate, NSWindowDelegate, NSTextViewDelegate, GameViewControllerProtocol>

@property BOOL isControlPressed;
@property BOOL isShiftPressed;
@property BOOL isCommandPressed;

- (void)onScriptEnter;
- (void)onScriptChange;

@end
