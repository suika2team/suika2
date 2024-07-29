/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#ifndef OPENNOVEL_ASOUND_H
#define OPENNOVEL_ASOUND_H

#include "../types.h"

/* Initialize ALSA. */
bool init_asound(void);

/* Cleanup ALSA. */
void cleanup_asound(void);

#endif
