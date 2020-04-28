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

#ifndef AMTK_MENU_SHELL_H
#define AMTK_MENU_SHELL_H

#if !defined (AMTK_H_INSIDE) && !defined (AMTK_COMPILATION)
#error "Only <amtk/amtk.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <amtk/amtk-types.h>

G_BEGIN_DECLS

#define AMTK_TYPE_MENU_SHELL             (amtk_menu_shell_get_type ())
#define AMTK_MENU_SHELL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), AMTK_TYPE_MENU_SHELL, AmtkMenuShell))
#define AMTK_MENU_SHELL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), AMTK_TYPE_MENU_SHELL, AmtkMenuShellClass))
#define AMTK_IS_MENU_SHELL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AMTK_TYPE_MENU_SHELL))
#define AMTK_IS_MENU_SHELL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), AMTK_TYPE_MENU_SHELL))
#define AMTK_MENU_SHELL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), AMTK_TYPE_MENU_SHELL, AmtkMenuShellClass))

typedef struct _AmtkMenuShellClass    AmtkMenuShellClass;
typedef struct _AmtkMenuShellPrivate  AmtkMenuShellPrivate;

struct _AmtkMenuShell
{
	GObject parent;

	AmtkMenuShellPrivate *priv;
};

struct _AmtkMenuShellClass
{
	GObjectClass parent_class;

	/* Signals */

	void (* menu_item_selected)	(AmtkMenuShell *amtk_menu_shell,
					 GtkMenuItem   *menu_item);

	void (* menu_item_deselected)	(AmtkMenuShell *amtk_menu_shell,
					 GtkMenuItem   *menu_item);

	gpointer padding[12];
};

_AMTK_EXTERN
GType		amtk_menu_shell_get_type		(void) G_GNUC_CONST;

_AMTK_EXTERN
AmtkMenuShell *	amtk_menu_shell_get_from_gtk_menu_shell	(GtkMenuShell *gtk_menu_shell);

_AMTK_EXTERN
GtkMenuShell *	amtk_menu_shell_get_menu_shell		(AmtkMenuShell *amtk_menu_shell);

G_END_DECLS

#endif /* AMTK_MENU_SHELL_H */

/* ex:set ts=8 noet: */
