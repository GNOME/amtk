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

#include "amtk-factory.h"
#include "amtk-action-info.h"
#include "amtk-action-info-central-store.h"
#include "amtk-menu-item.h"
#include "amtk-utils.h"
#include "amtk-enum-types.h"

/**
 * SECTION:amtk-factory
 * @Short_description: Factory functions
 * @Title: AmtkFactory
 *
 * #AmtkFactory contains functions to create various objects such as a menu
 * item, a toolbar item or a #GtkShortcutsShortcut, from the information
 * provided by an #AmtkActionInfo. A factory function accesses an
 * #AmtkActionInfo from the #AmtkActionInfoCentralStore.
 *
 * A #GtkApplication can be associated so that factory functions can call
 * gtk_application_set_accels_for_action() with the accelerators returned by
 * amtk_action_info_get_accels(). Note that
 * gtk_application_set_accels_for_action() is called by factory functions and
 * not by amtk_action_info_store_add(), so that libraries can provide their own
 * store and the accelerators are set to the #GtkApplication only if an
 * #AmtkActionInfo is actually used. For an application store,
 * amtk_action_info_store_set_all_accels_to_app() is an alternative.
 *
 * #AmtkFactoryFlags permits to control how a factory function creates the
 * object, to ignore some steps. Factory functions are declined in two variants:
 * a simple form which takes the value of the #AmtkFactory:default-flags
 * property, and the same function with the `_full` suffix which takes an
 * #AmtkFactoryFlags argument and ignores the #AmtkFactory:default-flags. See
 * for example amtk_factory_create_menu_item() and
 * amtk_factory_create_menu_item_full().
 *
 * # Static objects # {#amtk-factory-static-objects}
 *
 * An important detail is that once a factory function has created an object,
 * the object is not updated if the corresponding #AmtkActionInfo is modified
 * afterwards. #AmtkActionInfo doesn't have a notify signal, and it is anyway
 * discouraged to modify an #AmtkActionInfo after being added to an
 * #AmtkActionInfoStore. An #AmtkActionInfoStore is meant to be something
 * static, created on application startup. Updating automatically menu and
 * toolbar items is out of scope for the Amtk library. If for example action
 * accelerators can be modified at run-time, the menu needs to be re-generated.
 *
 * # Menus
 *
 * Some general notes about the functions that create #GtkMenuItem's:
 * - If not ignored by an #AmtkFactoryFlags, the first accelerator returned by
 *   amtk_action_info_get_accels() is set to the #GtkAccelLabel of the
 *   #GtkMenuItem.
 * - If not ignored by an #AmtkFactoryFlags, the tooltip is set with
 *   amtk_menu_item_set_long_description(), which permits to display it in a
 *   #GtkStatusbar with amtk_application_window_connect_menu_to_statusbar().
 *
 * ## Code example to create a menu
 *
 * How to create a #GtkMenuBar with #AmtkFactory. Each submenu is created by a
 * separate function, to make the code clearer.
 *
 * |[
 * static GtkWidget *
 * create_file_submenu (void)
 * {
 *   GtkMenuShell *file_submenu;
 *   AmtkFactory *factory;
 *
 *   file_submenu = GTK_MENU_SHELL (gtk_menu_new ());
 *
 *   factory = amtk_factory_new_with_default_application ();
 *   gtk_menu_shell_append (file_submenu, amtk_factory_create_menu_item (factory, "win.open"));
 *   gtk_menu_shell_append (file_submenu, amtk_factory_create_menu_item (factory, "win.save"));
 *   gtk_menu_shell_append (file_submenu, gtk_separator_menu_item_new ());
 *   gtk_menu_shell_append (file_submenu, amtk_factory_create_menu_item (factory, "app.quit"));
 *   g_object_unref (factory);
 *
 *   return GTK_WIDGET (file_submenu);
 * }
 *
 * static GtkWidget *
 * create_help_submenu (void)
 * {
 *   GtkMenuShell *help_submenu;
 *   AmtkFactory *factory;
 *
 *   help_submenu = GTK_MENU_SHELL (gtk_menu_new ());
 *
 *   factory = amtk_factory_new_with_default_application ();
 *   gtk_menu_shell_append (help_submenu, amtk_factory_create_menu_item (factory, "app.about"));
 *   g_object_unref (factory);
 *
 *   return GTK_WIDGET (help_submenu);
 * }
 *
 * static GtkWidget *
 * create_menu_bar (void)
 * {
 *   GtkWidget *file_menu_item;
 *   GtkWidget *help_menu_item;
 *   GtkWidget *menu_bar;
 *
 *   file_menu_item = gtk_menu_item_new_with_mnemonic ("_File");
 *   gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item),
 *                              create_file_submenu ());
 *
 *   help_menu_item = gtk_menu_item_new_with_mnemonic ("_Help");
 *   gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu_item),
 *                              create_help_submenu ());
 *
 *   menu_bar = gtk_menu_bar_new ();
 *   gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_menu_item);
 *   gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), help_menu_item);
 *
 *   // Additionally, it is a good place to call
 *   // amtk_action_info_store_check_all_used() here.
 *
 *   return menu_bar;
 * }
 * ]|
 *
 * # Toolbars
 *
 * Some general notes about the functions that create #GtkToolItem's:
 * - The accelerators returned by amtk_action_info_get_accels() are not used for
 *   documentation purposes in a toolbar, so the
 *   %AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC flag has no impact. The accelerators
 *   are documented only in menu items.
 *
 * ## Code example to create a toolbar
 *
 * How to create a basic #GtkToolbar with #AmtkFactory.
 *
 * |[
 * static GtkWidget *
 * create_toolbar (void)
 * {
 *   GtkToolbar *toolbar;
 *   AmtkFactory *factory;
 *
 *   toolbar = GTK_TOOLBAR (gtk_toolbar_new ());
 *
 *   // Small performance improvement:
 *   // Do not associate a GtkApplication, because the menu has already been
 *   // generated, the menu contains all actions, so
 *   // gtk_application_set_accels_for_action() has already been called for all
 *   // actions. Another way is to set the AMTK_FACTORY_IGNORE_ACCELS_FOR_APP
 *   // flag.
 *   factory = amtk_factory_new (NULL);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.new-file"), -1);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.open"), -1);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.save"), -1);
 *   gtk_toolbar_insert (toolbar, gtk_separator_tool_item_new (), -1);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.cut"), -1);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.copy"), -1);
 *   gtk_toolbar_insert (toolbar, amtk_factory_create_tool_button (factory, "win.paste"), -1);
 *   g_object_unref (factory);
 *
 *   return GTK_WIDGET (toolbar);
 * }
 * ]|
 *
 * # Modern UI with a GtkHeaderBar
 *
 * To have an example code, see `tests/test-headerbar.c` in the git repository
 * of Amtk.
 */

struct _AmtkFactoryPrivate
{
	/* Weak ref */
	GtkApplication *app;

	AmtkFactoryFlags default_flags;
};

enum
{
	PROP_0,
	PROP_APPLICATION,
	PROP_DEFAULT_FLAGS,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (AmtkFactory, amtk_factory, G_TYPE_OBJECT)

static void
amtk_factory_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
	AmtkFactory *factory = AMTK_FACTORY (object);

	switch (prop_id)
	{
		case PROP_APPLICATION:
			g_value_set_object (value, amtk_factory_get_application (factory));
			break;

		case PROP_DEFAULT_FLAGS:
			g_value_set_flags (value, amtk_factory_get_default_flags (factory));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
amtk_factory_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
	AmtkFactory *factory = AMTK_FACTORY (object);

	switch (prop_id)
	{
		case PROP_APPLICATION:
			g_assert (factory->priv->app == NULL);
			g_set_weak_pointer (&factory->priv->app, g_value_get_object (value));
			break;

		case PROP_DEFAULT_FLAGS:
			amtk_factory_set_default_flags (factory, g_value_get_flags (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
amtk_factory_dispose (GObject *object)
{
	AmtkFactory *factory = AMTK_FACTORY (object);

	g_clear_weak_pointer (&factory->priv->app);

	G_OBJECT_CLASS (amtk_factory_parent_class)->dispose (object);
}

static void
amtk_factory_class_init (AmtkFactoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = amtk_factory_get_property;
	object_class->set_property = amtk_factory_set_property;
	object_class->dispose = amtk_factory_dispose;

	/**
	 * AmtkFactory:application:
	 *
	 * The associated #GtkApplication (it is optional, it can be %NULL).
	 * #AmtkFactory has a weak reference to the #GtkApplication.
	 *
	 * Since: 3.0
	 */
	properties[PROP_APPLICATION] =
		g_param_spec_object ("application",
				     "GtkApplication",
				     "",
				     GTK_TYPE_APPLICATION,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	/**
	 * AmtkFactory:default-flags:
	 *
	 * The default #AmtkFactoryFlags.
	 *
	 * Since: 3.0
	 */
	properties[PROP_DEFAULT_FLAGS] =
		g_param_spec_flags ("default-flags",
				    "Default flags",
				    "",
				    AMTK_TYPE_FACTORY_FLAGS,
				    AMTK_FACTORY_FLAGS_NONE,
				    G_PARAM_READWRITE |
				    G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
amtk_factory_init (AmtkFactory *factory)
{
	factory->priv = amtk_factory_get_instance_private (factory);
}

/**
 * amtk_factory_new:
 * @application: (nullable): a #GtkApplication, or %NULL.
 *
 * Creates a new #AmtkFactory object. Associating a #GtkApplication is optional,
 * if it is %NULL gtk_application_set_accels_for_action() won't be called.
 *
 * Returns: a new #AmtkFactory.
 * Since: 3.0
 */
AmtkFactory *
amtk_factory_new (GtkApplication *application)
{
	g_return_val_if_fail (application == NULL || GTK_IS_APPLICATION (application), NULL);

	return g_object_new (AMTK_TYPE_FACTORY,
			     "application", application,
			     NULL);
}

/**
 * amtk_factory_new_with_default_application:
 *
 * Calls amtk_factory_new() with g_application_get_default() (it must be a
 * #GtkApplication).
 *
 * Returns: a new #AmtkFactory with the default #GtkApplication.
 * Since: 3.0
 */
AmtkFactory *
amtk_factory_new_with_default_application (void)
{
	return amtk_factory_new (GTK_APPLICATION (g_application_get_default ()));
}

/**
 * amtk_factory_get_application:
 * @factory: an #AmtkFactory.
 *
 * Returns: (transfer none) (nullable): the #AmtkFactory:application.
 * Since: 3.0
 */
GtkApplication *
amtk_factory_get_application (AmtkFactory *factory)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);

	return factory->priv->app;
}

/**
 * amtk_factory_get_default_flags:
 * @factory: an #AmtkFactory.
 *
 * Returns: the #AmtkFactory:default-flags.
 * Since: 3.0
 */
AmtkFactoryFlags
amtk_factory_get_default_flags (AmtkFactory *factory)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), AMTK_FACTORY_FLAGS_NONE);

	return factory->priv->default_flags;
}

/**
 * amtk_factory_set_default_flags:
 * @factory: an #AmtkFactory.
 * @default_flags: the new value.
 *
 * Sets the #AmtkFactory:default-flags property.
 *
 * Since: 3.0
 */
void
amtk_factory_set_default_flags (AmtkFactory      *factory,
				AmtkFactoryFlags  default_flags)
{
	g_return_if_fail (AMTK_IS_FACTORY (factory));

	if (factory->priv->default_flags != default_flags)
	{
		factory->priv->default_flags = default_flags;
		g_object_notify_by_pspec (G_OBJECT (factory), properties[PROP_DEFAULT_FLAGS]);
	}
}

static AmtkActionInfo *
common_create (AmtkFactory      *factory,
	       const gchar      *action_name,
	       AmtkFactoryFlags  flags)
{
	AmtkActionInfoCentralStore *central_store;
	AmtkActionInfo *action_info;

	central_store = amtk_action_info_central_store_get_singleton ();
	action_info = amtk_action_info_central_store_lookup (central_store, action_name);

	if (action_info == NULL)
	{
		g_warning ("AmtkFactory create function: action name '%s' not found.",
			   action_name);
		return NULL;
	}

	if ((flags & AMTK_FACTORY_IGNORE_ACCELS) == 0 &&
	    (flags & AMTK_FACTORY_IGNORE_ACCELS_FOR_APP) == 0 &&
	    factory->priv->app != NULL)
	{
		const gchar * const *accels;

		accels = amtk_action_info_get_accels (action_info);
		gtk_application_set_accels_for_action (factory->priv->app, action_name, accels);
	}

	amtk_action_info_mark_as_used (action_info);

	return action_info;
}

static AmtkActionInfo *
common_create_widget (AmtkFactory       *factory,
		      const gchar       *action_name,
		      AmtkFactoryFlags   flags,
		      GtkWidget        **widget)
{
	AmtkActionInfo *action_info;

	action_info = common_create (factory, action_name, flags);

	if (action_info == NULL)
	{
		g_object_ref_sink (*widget);
		g_object_unref (*widget);
		*widget = NULL;
		return NULL;
	}

	if ((flags & AMTK_FACTORY_IGNORE_GACTION) == 0)
	{
		gtk_actionable_set_detailed_action_name (GTK_ACTIONABLE (*widget), action_name);
	}

	return action_info;
}

static AmtkActionInfo *
common_create_menu_item (AmtkFactory       *factory,
			 const gchar       *action_name,
			 AmtkFactoryFlags   flags,
			 GtkMenuItem      **menu_item)
{
	AmtkActionInfo *action_info;
	const gchar * const *accels;
	const gchar *tooltip;

	action_info = common_create_widget (factory, action_name, flags, (GtkWidget **)menu_item);
	if (action_info == NULL)
	{
		return NULL;
	}

	if ((flags & AMTK_FACTORY_IGNORE_LABEL) == 0)
	{
		gtk_menu_item_set_use_underline (*menu_item, TRUE);
		gtk_menu_item_set_label (*menu_item, amtk_action_info_get_label (action_info));
	}

	accels = amtk_action_info_get_accels (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_ACCELS) == 0 &&
	    (flags & AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC) == 0 &&
	    accels != NULL && accels[0] != NULL)
	{
		guint accel_key;
		GdkModifierType accel_mods;

		gtk_accelerator_parse (accels[0], &accel_key, &accel_mods);

		if (accel_key != 0 || accel_mods != 0)
		{
			GtkWidget *child;

			child = gtk_bin_get_child (GTK_BIN (*menu_item));

			gtk_accel_label_set_accel (GTK_ACCEL_LABEL (child),
						   accel_key,
						   accel_mods);
		}
	}

	tooltip = amtk_action_info_get_tooltip (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_TOOLTIP) == 0 &&
	    tooltip != NULL)
	{
		amtk_menu_item_set_long_description (*menu_item, tooltip);
	}

	return action_info;
}

static AmtkActionInfo *
common_create_tool_button (AmtkFactory       *factory,
			   const gchar       *action_name,
			   AmtkFactoryFlags   flags,
			   GtkToolButton    **tool_button)
{
	AmtkActionInfo *action_info;
	const gchar *icon_name;
	const gchar *tooltip;

	action_info = common_create_widget (factory, action_name, flags, (GtkWidget **)tool_button);
	if (action_info == NULL)
	{
		return NULL;
	}

	if ((flags & AMTK_FACTORY_IGNORE_LABEL) == 0)
	{
		gtk_tool_button_set_use_underline (*tool_button, TRUE);
		gtk_tool_button_set_label (*tool_button, amtk_action_info_get_label (action_info));
	}

	icon_name = amtk_action_info_get_icon_name (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_ICON) == 0 &&
	    icon_name != NULL)
	{
		gtk_tool_button_set_icon_name (*tool_button, icon_name);
	}

	tooltip = amtk_action_info_get_tooltip (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_TOOLTIP) == 0 &&
	    tooltip != NULL)
	{
		gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM (*tool_button), tooltip);
	}

	return action_info;
}

/**
 * amtk_factory_create_menu_item:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Creates a new #GtkMenuItem for @action_name with the
 * #AmtkFactory:default-flags.
 *
 * Returns: (transfer floating): a new #GtkMenuItem for @action_name.
 * Since: 3.0
 */
GtkWidget *
amtk_factory_create_menu_item (AmtkFactory *factory,
			       const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_menu_item_full (factory,
						   action_name,
						   factory->priv->default_flags);
}

/**
 * amtk_factory_create_menu_item_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * Returns: (transfer floating): a new #GtkMenuItem for @action_name.
 * Since: 3.0
 */
GtkWidget *
amtk_factory_create_menu_item_full (AmtkFactory      *factory,
				    const gchar      *action_name,
				    AmtkFactoryFlags  flags)
{
	GtkMenuItem *menu_item;
	AmtkActionInfo *action_info;
	const gchar *icon_name;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	menu_item = GTK_MENU_ITEM (gtk_menu_item_new ());

	action_info = common_create_menu_item (factory, action_name, flags, &menu_item);
	if (action_info == NULL)
	{
		return NULL;
	}

	icon_name = amtk_action_info_get_icon_name (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_ICON) == 0 &&
	    icon_name != NULL)
	{
		amtk_menu_item_set_icon_name (menu_item, icon_name);
	}

	return GTK_WIDGET (menu_item);
}

/**
 * amtk_factory_create_check_menu_item:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Creates a new #GtkCheckMenuItem for @action_name with the
 * #AmtkFactory:default-flags.
 *
 * See the documentation of amtk_factory_create_check_menu_item_full() for more
 * information.
 *
 * Returns: (transfer floating): a new #GtkCheckMenuItem for @action_name.
 * Since: 3.0
 */
GtkWidget *
amtk_factory_create_check_menu_item (AmtkFactory *factory,
				     const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_check_menu_item_full (factory,
							 action_name,
							 factory->priv->default_flags);
}

/**
 * amtk_factory_create_check_menu_item_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * Note that since it is a #GtkCheckMenuItem the icon is not set, even if it
 * would be possible with amtk_menu_item_set_icon_name().
 *
 * If the action controls a boolean property, think about using
 * #GPropertyAction.
 *
 * Returns: (transfer floating): a new #GtkCheckMenuItem for @action_name.
 * Since: 3.0
 */
GtkWidget *
amtk_factory_create_check_menu_item_full (AmtkFactory      *factory,
					  const gchar      *action_name,
					  AmtkFactoryFlags  flags)
{
	GtkMenuItem *check_menu_item;
	AmtkActionInfo *action_info;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	check_menu_item = GTK_MENU_ITEM (gtk_check_menu_item_new ());

	action_info = common_create_menu_item (factory, action_name, flags, &check_menu_item);
	if (action_info == NULL)
	{
		return NULL;
	}

	return GTK_WIDGET (check_menu_item);
}

/**
 * amtk_factory_create_simple_menu:
 * @factory: an #AmtkFactory.
 * @entries: (array length=n_entries) (element-type AmtkActionInfoEntry): a
 *   pointer to the first item in an array of #AmtkActionInfoEntry structs.
 * @n_entries: the length of @entries, or -1 if @entries is %NULL-terminated.
 *
 * Calls amtk_factory_create_simple_menu_full() with the
 * #AmtkFactory:default-flags.
 *
 * Returns: (transfer floating): a new simple #GtkMenu for @entries.
 * Since: 5.0
 */
GtkWidget *
amtk_factory_create_simple_menu (AmtkFactory               *factory,
				 const AmtkActionInfoEntry *entries,
				 gint                       n_entries)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (n_entries >= -1, NULL);
	g_return_val_if_fail (entries != NULL || n_entries == 0, NULL);

	return amtk_factory_create_simple_menu_full (factory,
						     entries,
						     n_entries,
						     factory->priv->default_flags);
}

/**
 * amtk_factory_create_simple_menu_full:
 * @factory: an #AmtkFactory.
 * @entries: (array length=n_entries) (element-type AmtkActionInfoEntry): a
 *   pointer to the first item in an array of #AmtkActionInfoEntry structs.
 * @n_entries: the length of @entries, or -1 if @entries is %NULL-terminated.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * This function:
 * - Creates a #GtkMenu;
 * - For each #AmtkActionInfoEntry action name from @entries, creates a
 *   #GtkMenuItem with amtk_factory_create_menu_item_full() with the same @flags
 *   as passed in to this function, and appends it to the #GtkMenu, in the same
 *   order as provided by the @entries array.
 *
 * So this function is useful only if the #GtkMenu contains only simple
 * #GtkMenuItem's, not #GtkCheckMenuItem's nor #GtkRadioMenuItem's.
 *
 * Returns: (transfer floating): a new simple #GtkMenu for @entries.
 * Since: 5.0
 */
GtkWidget *
amtk_factory_create_simple_menu_full (AmtkFactory               *factory,
				      const AmtkActionInfoEntry *entries,
				      gint                       n_entries,
				      AmtkFactoryFlags           flags)
{
	GtkMenuShell *menu;
	gint i;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (n_entries >= -1, NULL);
	g_return_val_if_fail (entries != NULL || n_entries == 0, NULL);

	menu = GTK_MENU_SHELL (gtk_menu_new ());

	for (i = 0; n_entries == -1 ? entries[i].action_name != NULL : i < n_entries; i++)
	{
		const AmtkActionInfoEntry *cur_entry = &entries[i];
		GtkWidget *menu_item;

		menu_item = amtk_factory_create_menu_item_full (factory, cur_entry->action_name, flags);
		gtk_menu_shell_append (menu, menu_item);
	}

	return GTK_WIDGET (menu);
}

/**
 * amtk_factory_create_gmenu_item:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Calls amtk_factory_create_gmenu_item_full() with the
 * #AmtkFactory:default-flags.
 *
 * Returns: (transfer full): a new #GMenuItem for @action_name.
 * Since: 5.0
 */
GMenuItem *
amtk_factory_create_gmenu_item (AmtkFactory *factory,
				const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_gmenu_item_full (factory,
						    action_name,
						    factory->priv->default_flags);
}

/**
 * amtk_factory_create_gmenu_item_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * Creates a new #GMenuItem for @action_name. It ignores the tooltip, i.e. the
 * return value of amtk_action_info_get_tooltip().
 *
 * Returns: (transfer full): a new #GMenuItem for @action_name.
 * Since: 5.0
 */
GMenuItem *
amtk_factory_create_gmenu_item_full (AmtkFactory      *factory,
				     const gchar      *action_name,
				     AmtkFactoryFlags  flags)
{
	AmtkActionInfo *action_info;
	const gchar *label = NULL;
	const gchar *detailed_action = NULL;
	GMenuItem *menu_item;
	const gchar *icon_name;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	action_info = common_create (factory, action_name, flags);
	if (action_info == NULL)
	{
		return NULL;
	}

	if ((flags & AMTK_FACTORY_IGNORE_LABEL) == 0)
	{
		label = amtk_action_info_get_label (action_info);
	}

	if ((flags & AMTK_FACTORY_IGNORE_GACTION) == 0)
	{
		detailed_action = action_name;
	}

	menu_item = g_menu_item_new (label, detailed_action);

	icon_name = amtk_action_info_get_icon_name (action_info);
	if ((flags & AMTK_FACTORY_IGNORE_ICON) == 0 &&
	    icon_name != NULL)
	{
		GIcon *icon;

		icon = g_themed_icon_new (icon_name);
		g_menu_item_set_icon (menu_item, icon);
		g_object_unref (icon);
	}

	return menu_item;
}

/**
 * amtk_factory_create_tool_button:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Creates a new #GtkToolButton for @action_name with the
 * #AmtkFactory:default-flags.
 *
 * Returns: (transfer floating): a new #GtkToolButton for @action_name.
 * Since: 3.0
 */
GtkToolItem *
amtk_factory_create_tool_button (AmtkFactory *factory,
				 const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_tool_button_full (factory,
						     action_name,
						     factory->priv->default_flags);
}

/**
 * amtk_factory_create_tool_button_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * Returns: (transfer floating): a new #GtkToolButton for @action_name.
 * Since: 3.0
 */
GtkToolItem *
amtk_factory_create_tool_button_full (AmtkFactory      *factory,
				      const gchar      *action_name,
				      AmtkFactoryFlags  flags)
{
	GtkToolButton *tool_button;
	AmtkActionInfo *action_info;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	tool_button = GTK_TOOL_BUTTON (gtk_tool_button_new (NULL, NULL));

	action_info = common_create_tool_button (factory, action_name, flags, &tool_button);
	if (action_info == NULL)
	{
		return NULL;
	}

	return GTK_TOOL_ITEM (tool_button);
}

/**
 * amtk_factory_create_menu_tool_button:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Creates a new #GtkMenuToolButton for @action_name with the
 * #AmtkFactory:default-flags.
 *
 * See the documentation of amtk_factory_create_menu_tool_button_full() for more
 * information.
 *
 * Returns: (transfer floating): a new #GtkMenuToolButton for @action_name.
 * Since: 3.0
 */
GtkMenuToolButton *
amtk_factory_create_menu_tool_button (AmtkFactory *factory,
				      const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_menu_tool_button_full (factory,
							  action_name,
							  factory->priv->default_flags);
}

/**
 * amtk_factory_create_menu_tool_button_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * After calling this function, you need to use the #GtkMenuToolButton API to
 * set the menu and also possibly set a tooltip to the arrow.
 *
 * Returns: (transfer floating): a new #GtkMenuToolButton for @action_name.
 * Since: 3.0
 */
GtkMenuToolButton *
amtk_factory_create_menu_tool_button_full (AmtkFactory      *factory,
					   const gchar      *action_name,
					   AmtkFactoryFlags  flags)
{
	GtkToolButton *menu_tool_button;
	AmtkActionInfo *action_info;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	menu_tool_button = GTK_TOOL_BUTTON (gtk_menu_tool_button_new (NULL, NULL));

	action_info = common_create_tool_button (factory, action_name, flags, &menu_tool_button);
	if (action_info == NULL)
	{
		return NULL;
	}

	return GTK_MENU_TOOL_BUTTON (menu_tool_button);
}

/**
 * amtk_factory_create_shortcut:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 *
 * Calls amtk_factory_create_shortcut_full() with the
 * #AmtkFactory:default-flags.
 *
 * Returns: (transfer floating): a new #GtkShortcutsShortcut for @action_name.
 * Since: 5.0
 */
GtkWidget *
amtk_factory_create_shortcut (AmtkFactory *factory,
			      const gchar *action_name)
{
	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	return amtk_factory_create_shortcut_full (factory,
						  action_name,
						  factory->priv->default_flags);
}

/**
 * amtk_factory_create_shortcut_full:
 * @factory: an #AmtkFactory.
 * @action_name: an action name.
 * @flags: #AmtkFactoryFlags.
 *
 * This function ignores the #AmtkFactory:default-flags property and takes the
 * @flags argument instead.
 *
 * This function creates a new #GtkShortcutsShortcut for @action_name.
 *
 * For the #GtkShortcutsShortcut:title, the tooltip has the priorioty, with the
 * label as fallback if the tooltip is %NULL (the mnemonic is removed from the
 * label with amtk_utils_remove_mnemonic()). This can be controlled with the
 * %AMTK_FACTORY_IGNORE_TOOLTIP and %AMTK_FACTORY_IGNORE_LABEL flags.
 *
 * The #GtkShortcutsShortcut:accelerator property is set with only the *first*
 * accel returned by amtk_action_info_get_accels(). This step can be ignored
 * with %AMTK_FACTORY_IGNORE_ACCELS or %AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC.
 *
 * The #GtkShortcutsShortcut:action-name property is set to @action_name if the
 * %AMTK_FACTORY_IGNORE_GACTION flag isn't set. Note that with
 * #GtkShortcutsShortcut:action-name all accelerators are displayed (if set to
 * the #GtkApplication).
 *
 * So depending on whether you want to show only the first accelerator or all
 * accelerators, you need to set @flags appropriately.
 *
 * Returns: (transfer floating): a new #GtkShortcutsShortcut for @action_name.
 * Since: 5.0
 */
GtkWidget *
amtk_factory_create_shortcut_full (AmtkFactory      *factory,
				   const gchar      *action_name,
				   AmtkFactoryFlags  flags)
{
	AmtkActionInfo *action_info;
	GtkWidget *shortcut;
	const gchar *tooltip;
	const gchar *label;

	g_return_val_if_fail (AMTK_IS_FACTORY (factory), NULL);
	g_return_val_if_fail (action_name != NULL, NULL);

	action_info = common_create (factory, action_name, flags);
	if (action_info == NULL)
	{
		return NULL;
	}

	shortcut = g_object_new (GTK_TYPE_SHORTCUTS_SHORTCUT, NULL);
	gtk_widget_show (shortcut);

	tooltip = amtk_action_info_get_tooltip (action_info);
	label = amtk_action_info_get_label (action_info);

	if ((flags & AMTK_FACTORY_IGNORE_TOOLTIP) == 0 &&
	    tooltip != NULL)
	{
		g_object_set (shortcut,
			      "title", tooltip,
			      NULL);
	}
	else if ((flags & AMTK_FACTORY_IGNORE_LABEL) == 0 &&
		 label != NULL)
	{
		gchar *label_without_mnemonic;

		label_without_mnemonic = amtk_utils_remove_mnemonic (label);

		g_object_set (shortcut,
			      "title", label_without_mnemonic,
			      NULL);

		g_free (label_without_mnemonic);
	}

	if ((flags & AMTK_FACTORY_IGNORE_ACCELS) == 0 &&
	    (flags & AMTK_FACTORY_IGNORE_ACCELS_FOR_DOC) == 0)
	{
		const gchar * const *accels;

		accels = amtk_action_info_get_accels (action_info);
		g_object_set (shortcut,
			      "accelerator", accels[0],
			      NULL);
	}

	if ((flags & AMTK_FACTORY_IGNORE_GACTION) == 0)
	{
		g_object_set (shortcut,
			      "action-name", action_name,
			      NULL);
	}

	return shortcut;
}
