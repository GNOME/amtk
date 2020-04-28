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

#ifndef AMTK_ACTION_INFO_CENTRAL_STORE_H
#define AMTK_ACTION_INFO_CENTRAL_STORE_H

#if !defined (AMTK_H_INSIDE) && !defined (AMTK_COMPILATION)
#error "Only <amtk/amtk.h> can be included directly."
#endif

#include <glib-object.h>
#include <amtk/amtk-types.h>

G_BEGIN_DECLS

#define AMTK_TYPE_ACTION_INFO_CENTRAL_STORE             (amtk_action_info_central_store_get_type ())
#define AMTK_ACTION_INFO_CENTRAL_STORE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), AMTK_TYPE_ACTION_INFO_CENTRAL_STORE, AmtkActionInfoCentralStore))
#define AMTK_ACTION_INFO_CENTRAL_STORE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), AMTK_TYPE_ACTION_INFO_CENTRAL_STORE, AmtkActionInfoCentralStoreClass))
#define AMTK_IS_ACTION_INFO_CENTRAL_STORE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AMTK_TYPE_ACTION_INFO_CENTRAL_STORE))
#define AMTK_IS_ACTION_INFO_CENTRAL_STORE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), AMTK_TYPE_ACTION_INFO_CENTRAL_STORE))
#define AMTK_ACTION_INFO_CENTRAL_STORE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), AMTK_TYPE_ACTION_INFO_CENTRAL_STORE, AmtkActionInfoCentralStoreClass))

typedef struct _AmtkActionInfoCentralStoreClass    AmtkActionInfoCentralStoreClass;
typedef struct _AmtkActionInfoCentralStorePrivate  AmtkActionInfoCentralStorePrivate;

struct _AmtkActionInfoCentralStore
{
	GObject parent;

	AmtkActionInfoCentralStorePrivate *priv;
};

struct _AmtkActionInfoCentralStoreClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

_AMTK_EXTERN
GType		amtk_action_info_central_store_get_type		(void) G_GNUC_CONST;

_AMTK_EXTERN
AmtkActionInfoCentralStore *
		amtk_action_info_central_store_get_singleton	(void);

_AMTK_EXTERN
AmtkActionInfo *amtk_action_info_central_store_lookup		(AmtkActionInfoCentralStore *central_store,
								 const gchar                *action_name);

G_GNUC_INTERNAL
void		_amtk_action_info_central_store_add		(AmtkActionInfoCentralStore *central_store,
								 AmtkActionInfo             *info);

G_GNUC_INTERNAL
void		_amtk_action_info_central_store_unref_singleton	(void);

G_END_DECLS

#endif /* AMTK_ACTION_INFO_CENTRAL_STORE_H */
