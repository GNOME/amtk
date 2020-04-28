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

#ifndef AMTK_APPLICATION_WINDOW_H
#define AMTK_APPLICATION_WINDOW_H

#if !defined (AMTK_H_INSIDE) && !defined (AMTK_COMPILATION)
#error "Only <amtk/amtk.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <amtk/amtk-types.h>

G_BEGIN_DECLS

#define AMTK_TYPE_APPLICATION_WINDOW             (amtk_application_window_get_type ())
#define AMTK_APPLICATION_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), AMTK_TYPE_APPLICATION_WINDOW, AmtkApplicationWindow))
#define AMTK_APPLICATION_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), AMTK_TYPE_APPLICATION_WINDOW, AmtkApplicationWindowClass))
#define AMTK_IS_APPLICATION_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AMTK_TYPE_APPLICATION_WINDOW))
#define AMTK_IS_APPLICATION_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), AMTK_TYPE_APPLICATION_WINDOW))
#define AMTK_APPLICATION_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), AMTK_TYPE_APPLICATION_WINDOW, AmtkApplicationWindowClass))

typedef struct _AmtkApplicationWindowClass    AmtkApplicationWindowClass;
typedef struct _AmtkApplicationWindowPrivate  AmtkApplicationWindowPrivate;

struct _AmtkApplicationWindow
{
	GObject parent;

	AmtkApplicationWindowPrivate *priv;
};

struct _AmtkApplicationWindowClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

_AMTK_EXTERN
GType			amtk_application_window_get_type			(void) G_GNUC_CONST;

_AMTK_EXTERN
AmtkApplicationWindow *	amtk_application_window_get_from_gtk_application_window	(GtkApplicationWindow *gtk_window);

_AMTK_EXTERN
GtkApplicationWindow *	amtk_application_window_get_application_window		(AmtkApplicationWindow *amtk_window);

_AMTK_EXTERN
GtkStatusbar *		amtk_application_window_get_statusbar			(AmtkApplicationWindow *amtk_window);

_AMTK_EXTERN
void			amtk_application_window_set_statusbar			(AmtkApplicationWindow *amtk_window,
										 GtkStatusbar          *statusbar);

_AMTK_EXTERN
void			amtk_application_window_connect_menu_to_statusbar	(AmtkApplicationWindow *amtk_window,
										 GtkMenuShell          *menu_shell);

_AMTK_EXTERN
void			amtk_application_window_connect_recent_chooser_menu_to_statusbar
										(AmtkApplicationWindow *amtk_window,
										 GtkRecentChooserMenu  *menu);

_AMTK_EXTERN
GtkWidget *		amtk_application_window_create_open_recent_menu		(AmtkApplicationWindow *amtk_window);

_AMTK_EXTERN
GtkWidget *		amtk_application_window_create_open_recent_menu_item	(AmtkApplicationWindow *amtk_window);

G_END_DECLS

#endif /* AMTK_APPLICATION_WINDOW_H */

/* ex:set ts=8 noet: */
