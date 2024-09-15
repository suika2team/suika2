/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#import "ViewController.h"
#import "GameView.h"
#import "GameRenderer.h"

// Base
#import "opennovel.h"

// HAL
#import "aunit.h"

static ViewController *theViewController;
static BOOL isContinuousSwipeEnabled;

@interface ViewController ()
@end

@implementation ViewController
{
    // The GameView for AppKit
    GameView *_view;

    // The GameRender (common for AppKit and UIKit)
    GameRenderer *_renderer;

    // The screen information
    float _screenScale;
    CGSize _screenSize;
    CGPoint _screenOffset;

    // The video player objects and status.
    AVPlayer *_avPlayer;
    AVPlayerLayer *_avPlayerLayer;
    BOOL _isVideoPlaying;
}

//
// Called when the view is loaded.
//
- (void)viewDidLoad {
    [super viewDidLoad];

    // Initialize the engine.
    if(!init_file())
        exit(1);
    if(!init_conf())
        exit(1);
    if(!init_aunit())
        exit(1);
    if(!on_event_init())
        exit(1);
    
    // Create an MTKView.
    _view = (GameView *)self.view;
    _view.enableSetNeedsDisplay = YES;
    _view.device = MTLCreateSystemDefaultDevice();
    _view.clearColor = MTLClearColorMake(0.0, 0, 0, 1.0);
    _renderer = [[GameRenderer alloc] initWithMetalKitView:_view andController:self];
    if(!_renderer) {
        NSLog(@"Renderer initialization failed");
        return;
    }
    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];
    _view.delegate = _renderer;
    [self updateViewport:_view.frame.size];

    // Set multi-touch.
    self.view.multipleTouchEnabled = YES;

    // Setup a rendering timer.
    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(timerFired:)
                                   userInfo:nil
                                    repeats:YES];
}

//
// Calculate the viewport size.
//
- (void)updateViewport:(CGSize)newViewSize {
    // If called before the view initialization.
    if (newViewSize.width == 0 || newViewSize.height == 0)
        return;

    // Get the aspect ratio of the game.
    float aspect = (float)conf_game_height / (float)conf_game_width;

    // 1. Use width-first.
    _screenSize.width = newViewSize.width;
    _screenSize.height = _screenSize.width * aspect;
    _screenScale = (float)conf_game_width / _screenSize.width;
    
    // 2. If the height is not enough, use height-first.
    if(_screenSize.height > newViewSize.height) {
        _screenSize.height = newViewSize.height;
        _screenSize.width = _screenSize.height / aspect;
        _screenScale = (float)conf_game_height / _screenSize.height;
    }
    
    // Calculate the scale factor and the margin for touch position.
    _view.left = (newViewSize.width - _screenSize.width) / 2.0f;
    _view.top = (newViewSize.height - _screenSize.height) / 2.0f;
    _view.scale = _screenScale;

    // Multiply the scale factor of MTKView.
    _screenScale *= _view.layer.contentsScale;
    _screenSize.width *= _view.layer.contentsScale;
    _screenSize.height *= _view.layer.contentsScale;
    _screenOffset.x = _view.left * _view.layer.contentsScale;
    _screenOffset.y = _view.top * _view.layer.contentsScale;
}

//
// Called when the view appeared.
//
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self updateViewport:_view.frame.size];
}

//
// Called every frame.
//
- (void)timerFired:(NSTimer *)timer {
    [_view setNeedsDisplay];
}

//
// GameViewControllerProtocol
//

//
// Get the screen scale.
//
- (float)screenScale {
    return _screenScale;
}

//
// Get the screen offset.
//
- (CGPoint)screenOffset {
    return _screenOffset;
}

//
// Get the screen size.
//
- (CGSize)screenSize {
    return _screenSize;
}

//
// Check whether we are playing back a video.
//
- (BOOL)isVideoPlaying {
    return _isVideoPlaying;
}

//
// Play a video.
//
- (void)playVideoWithPath:(NSString *)path skippable:(BOOL)isSkippable {
    // Create a player.
    AVPlayerItem *playerItem = [[AVPlayerItem alloc] initWithURL:[NSURL fileURLWithPath:path]];
    _avPlayer = [[AVPlayer alloc] initWithPlayerItem:playerItem];

    // Create a layer for the player.
    _avPlayerLayer = [AVPlayerLayer playerLayerWithPlayer:_avPlayer];
    [_avPlayerLayer setFrame:self.view.bounds];
    [self.view.layer addSublayer:_avPlayerLayer];

    // Set the finish notification.
    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(onPlayEnd:)
                                               name:AVPlayerItemDidPlayToEndTimeNotification
                                             object:playerItem];

    // Start playing.
    [_avPlayer play];

    _isVideoPlaying = YES;
}

//
// Called when a video playback is finished.
//
- (void)onPlayEnd:(NSNotification *)notification {
    [_avPlayer replaceCurrentItemWithPlayerItem:nil];
    _isVideoPlaying = NO;
}

//
// Stop a video playback.
//
- (void)stopVideo {
    if (_avPlayer != nil) {
        [_avPlayer replaceCurrentItemWithPlayerItem:nil];
        _isVideoPlaying = NO;
        _avPlayer = nil;
        _avPlayerLayer = nil;
    }
}

//
// Not used in the iOS app.
//
- (void)setWindowTitle:(NSString *)name {
}

//
// Not used in the iOS app.
//
- (void)enterFullScreen {
}


//
// Not used in the iOS app.
//
- (BOOL)isFullScreen { 
    return NO;
}


//
// Not used in the iOS app.
//
- (void)leaveFullScreen { 
}


//
// Not used in the iOS app.
//
- (CGPoint)windowPointToScreenPoint:(CGPoint)windowPoint { 
    return windowPoint;
}

@end

//
// HAL
//

//
// Show an INFO log.
//
bool log_info(const char *s, ...)
{
    char buf[1024];
    va_list ap;
    
    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    NSLog(@"%s", buf);
    
    return true;
}

//
// Show a WARN log.
//
bool log_warn(const char *s, ...)
{
    char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    NSLog(@"%s", buf);

    return true;
}

//
// Show an ERROR log.
//
bool log_error(const char *s, ...)
{
    char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    NSLog(@"%s", buf);

    return true;
}

//
// Make a save directory.
//
bool make_sav_dir(void)
{
    @autoreleasepool {
        NSString *path = [NSString stringWithFormat:@"%@/%@/%s/sav",
                          NSHomeDirectory(),
                          @"/Library/Application Support",
                          conf_game_title];
        NSFileManager *manager = [NSFileManager defaultManager];
        NSError *error;
        if(![manager createDirectoryAtPath:path
               withIntermediateDirectories:YES
                                attributes:nil
                                     error:&error]) {
            NSLog(@"createDirectoryAtPath error: %@", error);
            return false;
        }
        return true;
    }
}

//
// Get a real path for a file.
//
char *make_valid_path(const char *dir, const char *fname)
{
    @autoreleasepool {
        // If a save file:
        if(dir != NULL && strcmp(dir, SAVE_DIR) == 0) {
            // Return a "Application Support" path.
            NSString *path = [NSString stringWithFormat:@"%@/%@/%s/save/%s",
                              NSHomeDirectory(),
                              @"/Library/Application Support",
                              conf_game_title,
                              fname];
            const char *cstr = [path UTF8String];
            return strdup(cstr);
        }

        // If the package.pak:
        if(dir == NULL && strcmp(fname, PACKAGE_FILE) == 0) {
            // Return a bundle resource path.
            NSString *path = [[NSBundle mainBundle] pathForResource:@"package" ofType:@"pak"];
            const char *cstr = [path UTF8String];
            return strdup(cstr);
        }

        // If an mp4 file:
        if(dir != NULL && strcmp(dir, MOV_DIR) == 0) {
            // Return an bundle resource path.
            *strstr(fname, ".") = '\0';
            NSString *basename = [NSString stringWithFormat:@"mov/%s", fname];
            NSString *path = [[NSBundle mainBundle] pathForResource:basename ofType:@"mp4"];
            const char *cstr = [path UTF8String];
            return strdup(cstr);
        }

        // We cannot load other files.
        return strdup("dummy");
    }
}

//
// This is used for long swipe.
//
void set_continuous_swipe_enabled(bool is_enabled)
{
	isContinuousSwipeEnabled = is_enabled;
}
