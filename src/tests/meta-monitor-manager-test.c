/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (C) 2016 Red Hat, Inc.
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
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "tests/meta-monitor-manager-test.h"

struct _MetaMonitorManagerTest
{
  MetaMonitorManager parent;

  MetaMonitorTestSetup *test_setup;
};

G_DEFINE_TYPE (MetaMonitorManagerTest, meta_monitor_manager_test,
               META_TYPE_MONITOR_MANAGER)

static MetaMonitorTestSetup *_initial_test_setup = NULL;

void
meta_monitor_manager_test_init_test_setup (MetaMonitorTestSetup *test_setup)
{
  _initial_test_setup = test_setup;
}

static void
meta_monitor_manager_test_read_current (MetaMonitorManager *manager)
{
  MetaMonitorManagerTest *manager_test = META_MONITOR_MANAGER_TEST (manager);

  manager->max_screen_width = 65535;
  manager->max_screen_height = 65535;

  g_assert (manager_test->test_setup);

  manager->modes = manager_test->test_setup->modes;
  manager->n_modes = manager_test->test_setup->n_modes;

  manager->crtcs = manager_test->test_setup->crtcs;
  manager->n_crtcs = manager_test->test_setup->n_crtcs;

  manager->outputs = manager_test->test_setup->outputs;
  manager->n_outputs = manager_test->test_setup->n_outputs;
}

static void
meta_monitor_manager_test_apply_configuration (MetaMonitorManager *manager,
                                               MetaCRTCInfo      **crtcs,
                                               unsigned int        n_crtcs,
                                               MetaOutputInfo    **outputs,
                                               unsigned int        n_outputs)
{
  unsigned int i;
  int screen_width = 0, screen_height = 0;

  for (i = 0; i < n_crtcs; i++)
    {
      MetaCRTCInfo *crtc_info = crtcs[i];
      MetaCRTC *crtc = crtc_info->crtc;
      crtc->is_dirty = TRUE;

      if (crtc_info->mode == NULL)
        {
          crtc->rect.x = 0;
          crtc->rect.y = 0;
          crtc->rect.width = 0;
          crtc->rect.height = 0;
          crtc->current_mode = NULL;
        }
      else
        {
          MetaMonitorMode *mode;
          MetaOutput *output;
          unsigned int j;
          int width, height;

          mode = crtc_info->mode;

          if (meta_monitor_transform_is_rotated (crtc_info->transform))
            {
              width = mode->height;
              height = mode->width;
            }
          else
            {
              width = mode->width;
              height = mode->height;
            }

          crtc->rect.x = crtc_info->x;
          crtc->rect.y = crtc_info->y;
          crtc->rect.width = width;
          crtc->rect.height = height;
          crtc->current_mode = mode;
          crtc->transform = crtc_info->transform;

          screen_width = MAX (screen_width, crtc_info->x + width);
          screen_height = MAX (screen_height, crtc_info->y + height);

          for (j = 0; j < crtc_info->outputs->len; j++)
            {
              output = ((MetaOutput**)crtc_info->outputs->pdata)[j];

              output->is_dirty = TRUE;
              output->crtc = crtc;
            }
        }
    }

  for (i = 0; i < n_outputs; i++)
    {
      MetaOutputInfo *output_info = outputs[i];
      MetaOutput *output = output_info->output;

      output->is_primary = output_info->is_primary;
      output->is_presentation = output_info->is_presentation;
    }

  /* Disable CRTCs not mentioned in the list */
  for (i = 0; i < manager->n_crtcs; i++)
    {
      MetaCRTC *crtc = &manager->crtcs[i];

      crtc->logical_monitor = NULL;

      if (crtc->is_dirty)
        {
          crtc->is_dirty = FALSE;
          continue;
        }

      crtc->rect.x = 0;
      crtc->rect.y = 0;
      crtc->rect.width = 0;
      crtc->rect.height = 0;
      crtc->current_mode = NULL;
    }

  /* Disable outputs not mentioned in the list */
  for (i = 0; i < manager->n_outputs; i++)
    {
      MetaOutput *output = &manager->outputs[i];

      if (output->is_dirty)
        {
          output->is_dirty = FALSE;
          continue;
        }

      output->crtc = NULL;
      output->is_primary = FALSE;
    }

  manager->screen_width = screen_width;
  manager->screen_height = screen_height;

  meta_monitor_manager_rebuild_derived (manager);
}

static void
meta_monitor_manager_test_init (MetaMonitorManagerTest *manager_test)
{
  g_assert (_initial_test_setup);

  manager_test->test_setup = _initial_test_setup;
}

static void
meta_monitor_manager_test_class_init (MetaMonitorManagerTestClass *klass)
{
  MetaMonitorManagerClass *manager_class = META_MONITOR_MANAGER_CLASS (klass);

  manager_class->read_current = meta_monitor_manager_test_read_current;
  manager_class->apply_configuration = meta_monitor_manager_test_apply_configuration;
}