/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * The Ciel Direction System
 */

#ifndef OPENNOVEL_CIEL_H
#define OPENNOVEL_CIEL_H

#include "file.h"

void ciel_clear_hook(void);
bool ciel_serialize_hook(struct wfile *wf);
bool ciel_deserialize_hook(struct rfile *rf);

#endif
