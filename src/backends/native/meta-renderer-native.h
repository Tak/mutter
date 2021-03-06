/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (C) 2016 Red Hat
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jonas Ådahl <jadahl@gmail.com>
 */

#ifndef META_RENDERER_NATIVE_H
#define META_RENDERER_NATIVE_H

#include <glib-object.h>
#include <xf86drmMode.h>

#include "backends/meta-renderer.h"

#define META_TYPE_RENDERER_NATIVE (meta_renderer_native_get_type ())
G_DECLARE_FINAL_TYPE (MetaRendererNative, meta_renderer_native,
                      META, RENDERER_NATIVE,
                      MetaRenderer)

typedef enum _MetaRendererNativeMode
{
  META_RENDERER_NATIVE_MODE_GBM,
#ifdef HAVE_EGL_DEVICE
  META_RENDERER_NATIVE_MODE_EGL_DEVICE
#endif
} MetaRendererNativeMode;

MetaRendererNative *meta_renderer_native_new (int         kms_fd,
                                              const char *kms_file_path,
                                              GError    **error);

MetaRendererNativeMode meta_renderer_native_get_mode (MetaRendererNative *renderer_native);

struct gbm_device * meta_renderer_native_get_gbm (MetaRendererNative *renderer_native);

int meta_renderer_native_get_kms_fd (MetaRendererNative *renderer_native);

void meta_renderer_native_queue_modes_reset (MetaRendererNative *renderer_native);

gboolean meta_renderer_native_set_legacy_view_size (MetaRendererNative *renderer_native,
                                                    MetaRendererView   *view,
                                                    int                 width,
                                                    int                 height,
                                                    GError            **error);

void meta_renderer_native_set_ignore_crtc (MetaRendererNative *renderer_native,
                                           uint32_t            id,
                                           gboolean            ignore);

MetaRendererView * meta_renderer_native_create_legacy_view (MetaRendererNative *renderer_native);

void meta_renderer_native_finish_frame (MetaRendererNative *renderer_native);

int64_t meta_renderer_native_get_frame_counter (MetaRendererNative *renderer_native);

void meta_renderer_native_pause (MetaRendererNative *renderer_native);

#endif /* META_RENDERER_NATIVE_H */
