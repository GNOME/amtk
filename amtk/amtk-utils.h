/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Amtk is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Amtk is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AMTK_UTILS_H
#define AMTK_UTILS_H

#if !defined (AMTK_H_INSIDE) && !defined (AMTK_COMPILATION)
#error "Only <amtk/amtk.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <amtk/amtk-types.h>

G_BEGIN_DECLS

/* File utilities */

G_GNUC_INTERNAL
gchar *		_amtk_utils_replace_home_dir_with_tilde		(const gchar *filename);

/* String utilities */

_AMTK_EXTERN
gchar *		amtk_utils_remove_mnemonic			(const gchar *str);

/* GTK utilities */

_AMTK_EXTERN
gchar *		amtk_utils_recent_chooser_menu_get_item_uri	(GtkRecentChooserMenu *menu,
								 GtkMenuItem          *item);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
_AMTK_EXTERN
void		amtk_utils_bind_g_action_to_gtk_action		(GActionMap     *g_action_map,
								 const gchar    *detailed_g_action_name_without_prefix,
								 GtkActionGroup *gtk_action_group,
								 const gchar    *gtk_action_name);

_AMTK_EXTERN
void		amtk_utils_create_gtk_action			(GActionMap     *g_action_map,
								 const gchar    *detailed_g_action_name_with_prefix,
								 GtkActionGroup *gtk_action_group,
								 const gchar    *gtk_action_name);
G_GNUC_END_IGNORE_DEPRECATIONS

G_END_DECLS

#endif /* AMTK_UTILS_H */
