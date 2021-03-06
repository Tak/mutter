/*
 * Copyright (C) 2017 Red Hat
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
 *
 */

#ifndef META_WAYLAND_INHIBIT_SHORTCUTS_DIALOG_H
#define META_WAYLAND_INHIBIT_SHORTCUTS_DIALOG_H

#include "meta-wayland-private.h"

void     meta_wayland_surface_show_inhibit_shortcuts_dialog (MetaWaylandSurface *surface,
                                                             MetaWaylandSeat    *seat);

void     meta_wayland_surface_hide_inhibit_shortcuts_dialog (MetaWaylandSurface *surface);

void     meta_wayland_surface_inhibit_shortcuts_dialog_init (void);

#endif /* META_WAYLAND_INHIBIT_SHORTCUTS_DIALOG_H */
