/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017, 2018, 2020 - Sébastien Wilmet <swilmet@gnome.org>
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

#ifndef AMTK_FACTORY_H
#define AMTK_FACTORY_H

#if !defined (AMTK_H_INSIDE) && !defined (AMTK_COMPILATION)
#error "Only <amtk/amtk.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <amtk/amtk-types.h>

G_BEGIN_DECLS

#define AMTK_TYPE_FACTORY             (amtk_factory_get_type ())
#define AMTK_FACTORY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), AMTK_TYPE_FACTORY, AmtkFactory))
#define AMTK_FACTORY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), AMTK_TYPE_FACTORY, AmtkFactoryClass))
#define AMTK_IS_FACTORY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AMTK_TYPE_FACTORY))
#define AMTK_IS_FACTORY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), AMTK_TYPE_FACTORY))
#define AMTK_FACTORY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), AMTK_TYPE_FACTORY, AmtkFactoryClass))

typedef struct _AmtkFactoryClass    AmtkFactoryClass;
typedef struct _AmtkFactoryPrivate  AmtkFactoryPrivate;

struct _AmtkFactory
{
	GObject parent;

	AmtkFactoryPrivate *priv;
};

struct _AmtkFactoryClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

/**
 * AmtkFactoryFlags:
 * @AMTK_FACTORY_FLAGS_NONE: No flags.
 * @AMTK_FACTORY_IGNORE_GACTION: Do not associate the created object with the
 *   #GAction. For example if the object to create is a #GtkActionable, do not
 *   call gtk_actionable_set_detailed_action_name().
 * @AMTK_FACTORY_IGNORE_ICON: Do not set an icon.
 * @AMTK_FACTORY_IGNORE_LABEL: Do not set a label/short description.
 * @AMTK_FACTORY_IGNORE_TOOLTIP: Do not set a tooltip/long description.
 * @AMTK_FACTORY_IGNORE_ACCELS: Ignore completely the accelerators.
 * @AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC: Ignore the accelerators for
 *   documentation purposes only. For example do not add/configure a
 *   #GtkAccelLabel.
 * @AMTK_FACTORY_IGNORE_ACCELS_FOR_APP: Do not call
 *   gtk_application_set_accels_for_action().
 *
 * #AmtkFactoryFlags permits to control how a factory function creates the
 * object, to ignore some steps.
 *
 * Since: 3.0
 */
typedef enum
{
	AMTK_FACTORY_FLAGS_NONE			= 0,
	AMTK_FACTORY_IGNORE_GACTION		= 1 << 0,
	AMTK_FACTORY_IGNORE_ICON		= 1 << 1,
	AMTK_FACTORY_IGNORE_LABEL		= 1 << 2,
	AMTK_FACTORY_IGNORE_TOOLTIP		= 1 << 3,
	AMTK_FACTORY_IGNORE_ACCELS		= 1 << 4,
	AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC	= 1 << 5,
	AMTK_FACTORY_IGNORE_ACCELS_FOR_APP	= 1 << 6,
} AmtkFactoryFlags;

_AMTK_EXTERN
GType			amtk_factory_get_type				(void);

_AMTK_EXTERN
AmtkFactory *		amtk_factory_new				(GtkApplication *application);

_AMTK_EXTERN
AmtkFactory *		amtk_factory_new_with_default_application	(void);

_AMTK_EXTERN
GtkApplication *	amtk_factory_get_application			(AmtkFactory *factory);

_AMTK_EXTERN
AmtkFactoryFlags	amtk_factory_get_default_flags			(AmtkFactory *factory);

_AMTK_EXTERN
void			amtk_factory_set_default_flags			(AmtkFactory      *factory,
									 AmtkFactoryFlags  default_flags);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_menu_item			(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_menu_item_full		(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_check_menu_item		(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_check_menu_item_full	(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_simple_menu			(AmtkFactory               *factory,
									 const AmtkActionInfoEntry *entries,
									 gint                       n_entries);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_simple_menu_full		(AmtkFactory               *factory,
									 const AmtkActionInfoEntry *entries,
									 gint                       n_entries,
									 AmtkFactoryFlags           flags);

_AMTK_EXTERN
GMenuItem *		amtk_factory_create_gmenu_item			(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GMenuItem *		amtk_factory_create_gmenu_item_full		(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

_AMTK_EXTERN
GtkToolItem *		amtk_factory_create_tool_button			(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GtkToolItem *		amtk_factory_create_tool_button_full		(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

_AMTK_EXTERN
GtkMenuToolButton *	amtk_factory_create_menu_tool_button		(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GtkMenuToolButton *	amtk_factory_create_menu_tool_button_full	(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_shortcut			(AmtkFactory *factory,
									 const gchar *action_name);

_AMTK_EXTERN
GtkWidget *		amtk_factory_create_shortcut_full		(AmtkFactory      *factory,
									 const gchar      *action_name,
									 AmtkFactoryFlags  flags);

G_END_DECLS

#endif /* AMTK_FACTORY_H */
