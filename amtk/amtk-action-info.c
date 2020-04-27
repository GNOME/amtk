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

#include "config.h"
#include "amtk-action-info.h"
#include <glib/gi18n-lib.h>
#include "amtk-utils.h"

/**
 * SECTION:amtk-action-info
 * @Short_description: #GAction information
 * @Title: AmtkActionInfo
 * @See_also: #AmtkActionInfoStore
 *
 * An #AmtkActionInfo instance contains a set of information about a #GAction.
 * Those pieces of information are useful to create UI elements that trigger the
 * #GAction, for example a menu item or a toolbar item.
 *
 * The recommended way to create a set of #AmtkActionInfo's is to use the
 * amtk_action_info_store_add_entries() function.
 */

struct _AmtkActionInfo
{
	gchar *action_name;
	gchar *icon_name;
	gchar *label;
	gchar *tooltip;

	/* Must never be NULL, must be a NULL-terminated array. This way, it
	 * can be used directly as an argument to
	 * gtk_application_set_accels_for_action().
	 */
	gchar **accels;

	gint ref_count;

	guint used : 1;
};

static void _amtk_action_info_free (AmtkActionInfo *info);

G_DEFINE_BOXED_TYPE (AmtkActionInfo, amtk_action_info,
		     amtk_action_info_copy,
		     _amtk_action_info_free)

static void
_amtk_action_info_free (AmtkActionInfo *info)
{
	if (info != NULL)
	{
		g_free (info->action_name);
		g_free (info->icon_name);
		g_free (info->label);
		g_free (info->tooltip);
		g_strfreev (info->accels);

		g_free (info);
	}
}

/**
 * amtk_action_info_new:
 *
 * Returns: a new #AmtkActionInfo.
 * Since: 2.0
 */
AmtkActionInfo *
amtk_action_info_new (void)
{
	AmtkActionInfo *info;

	info = g_new0 (AmtkActionInfo, 1);
	info->accels = g_malloc0 (sizeof (gchar *));
	info->ref_count = 1;

	return info;
}

/**
 * amtk_action_info_new_from_entry:
 * @info_entry: an #AmtkActionInfoEntry.
 * @translation_domain: (nullable): a gettext domain, or %NULL.
 *
 * Creates a new #AmtkActionInfo from an #AmtkActionInfoEntry.
 *
 * If @translation_domain is not %NULL, g_dgettext() is used to translate the
 * @label and @tooltip before setting them to the #AmtkActionInfo.
 *
 * Returns: a new #AmtkActionInfo.
 * Since: 2.0
 */
AmtkActionInfo *
amtk_action_info_new_from_entry (const AmtkActionInfoEntry *info_entry,
				 const gchar               *translation_domain)
{
	AmtkActionInfo *info;

	info = amtk_action_info_new ();
	info->action_name = g_strdup (info_entry->action_name);
	info->icon_name = g_strdup (info_entry->icon_name);

	if (translation_domain != NULL)
	{
		info->label = g_strdup (g_dgettext (translation_domain, info_entry->label));
		info->tooltip = g_strdup (g_dgettext (translation_domain, info_entry->tooltip));
	}
	else
	{
		info->label = g_strdup (info_entry->label);
		info->tooltip = g_strdup (info_entry->tooltip);
	}

	if (info_entry->accel != NULL)
	{
		g_strfreev (info->accels);

		info->accels = g_malloc (2 * sizeof (gchar *));
		info->accels[0] = g_strdup (info_entry->accel);
		info->accels[1] = NULL;
	}

	return info;
}

/**
 * amtk_action_info_ref:
 * @info: an #AmtkActionInfo.
 *
 * Increments the reference count of @info by one.
 *
 * Returns: the passed in @info.
 * Since: 2.0
 */
AmtkActionInfo *
amtk_action_info_ref (AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	info->ref_count++;

	return info;
}

/**
 * amtk_action_info_unref:
 * @info: an #AmtkActionInfo.
 *
 * Decrements the reference count of @info by one. If the reference count drops
 * to 0, @info is freed.
 *
 * Since: 2.0
 */
void
amtk_action_info_unref (AmtkActionInfo *info)
{
	g_return_if_fail (info != NULL);

	info->ref_count--;

	if (info->ref_count == 0)
	{
		_amtk_action_info_free (info);
	}
}

/**
 * amtk_action_info_copy:
 * @info: an #AmtkActionInfo.
 *
 * Returns: (transfer full): a copy of @info. The copy will have a reference
 * count of one.
 * Since: 2.0
 */
AmtkActionInfo *
amtk_action_info_copy (const AmtkActionInfo *info)
{
	AmtkActionInfo *new_info;

	g_return_val_if_fail (info != NULL, NULL);

	new_info = amtk_action_info_new ();

	new_info->action_name = g_strdup (info->action_name);
	new_info->icon_name = g_strdup (info->icon_name);
	new_info->label = g_strdup (info->label);
	new_info->tooltip = g_strdup (info->tooltip);

	amtk_action_info_set_accels (new_info, (const gchar * const *)info->accels);

	return new_info;
}

/**
 * amtk_action_info_get_action_name:
 * @info: an #AmtkActionInfo.
 *
 * Returns: (nullable): the action name, or %NULL. Example: `"win.save"`. Can be
 * a detailed action name, see g_action_parse_detailed_name().
 * Since: 2.0
 */
const gchar *
amtk_action_info_get_action_name (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->action_name;
}

/**
 * amtk_action_info_set_action_name:
 * @info: an #AmtkActionInfo.
 * @action_name: the action name.
 *
 * Sets the action name, for example `"win.save"`. Can be a detailed action
 * name, see g_action_parse_detailed_name().
 *
 * Since: 2.0
 */
void
amtk_action_info_set_action_name (AmtkActionInfo *info,
				  const gchar    *action_name)
{
	g_return_if_fail (info != NULL);
	g_return_if_fail (action_name != NULL);

	g_free (info->action_name);
	info->action_name = g_strdup (action_name);
}

/**
 * amtk_action_info_get_icon_name:
 * @info: an #AmtkActionInfo.
 *
 * Returns: (nullable): the icon name, or %NULL.
 * Since: 2.0
 */
const gchar *
amtk_action_info_get_icon_name (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->icon_name;
}

/**
 * amtk_action_info_set_icon_name:
 * @info: an #AmtkActionInfo.
 * @icon_name: (nullable): the icon name, or %NULL.
 *
 * Since: 2.0
 */
void
amtk_action_info_set_icon_name (AmtkActionInfo *info,
				const gchar    *icon_name)
{
	g_return_if_fail (info != NULL);

	g_free (info->icon_name);
	info->icon_name = g_strdup (icon_name);
}

/**
 * amtk_action_info_get_label:
 * @info: an #AmtkActionInfo.
 *
 * Gets the label. The label has normally a mnemonic. To remove the mnemonic,
 * there is the amtk_utils_remove_mnemonic() function.
 *
 * Returns: (nullable): the label (i.e. a short description), or %NULL.
 * Since: 2.0
 */
const gchar *
amtk_action_info_get_label (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->label;
}

/**
 * amtk_action_info_set_label:
 * @info: an #AmtkActionInfo.
 * @label: (nullable): the label (i.e. a short description), or %NULL.
 *
 * Sets the label with a mnemonic. To know how to encode the mnemonic, see the
 * documentation of gtk_label_new_with_mnemonic().
 *
 * Since: 2.0
 */
void
amtk_action_info_set_label (AmtkActionInfo *info,
			    const gchar    *label)
{
	g_return_if_fail (info != NULL);

	g_free (info->label);
	info->label = g_strdup (label);
}

/**
 * amtk_action_info_get_tooltip:
 * @info: an #AmtkActionInfo.
 *
 * Returns: (nullable): the tooltip (i.e. a long description), or %NULL.
 * Since: 2.0
 */
const gchar *
amtk_action_info_get_tooltip (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->tooltip;
}

/**
 * amtk_action_info_set_tooltip:
 * @info: an #AmtkActionInfo.
 * @tooltip: (nullable): the tooltip (i.e. a long description), or %NULL.
 *
 * Since: 2.0
 */
void
amtk_action_info_set_tooltip (AmtkActionInfo *info,
			      const gchar    *tooltip)
{
	g_return_if_fail (info != NULL);

	g_free (info->tooltip);
	info->tooltip = g_strdup (tooltip);
}

/**
 * amtk_action_info_get_accels:
 * @info: an #AmtkActionInfo.
 *
 * Returns the accelerators. This function never returns %NULL, it always
 * returns a %NULL-terminated array, to be suitable for
 * gtk_application_set_accels_for_action().
 *
 * Returns: (transfer none) (array zero-terminated=1): a %NULL-terminated array
 * of accelerators in the format understood by gtk_accelerator_parse().
 * Since: 2.0
 */
const gchar * const *
amtk_action_info_get_accels (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	g_assert (info->accels != NULL);

	return (const gchar * const *)info->accels;
}

/**
 * amtk_action_info_set_accels:
 * @info: an #AmtkActionInfo.
 * @accels: (array zero-terminated=1): a %NULL-terminated array of accelerators
 * in the format understood by gtk_accelerator_parse().
 *
 * A function similar to gtk_application_set_accels_for_action().
 *
 * @accels must not be %NULL, it must be a %NULL-terminated array, to be
 * consistent with gtk_application_set_accels_for_action().
 *
 * Since: 2.0
 */
void
amtk_action_info_set_accels (AmtkActionInfo      *info,
			     const gchar * const *accels)
{
	g_return_if_fail (info != NULL);
	g_return_if_fail (accels != NULL);

	g_strfreev (info->accels);
	info->accels = g_strdupv ((gchar **) accels);
}

/**
 * amtk_action_info_mark_as_used:
 * @info: an #AmtkActionInfo.
 *
 * Mark @info as used. An #AmtkFactory function that uses an #AmtkActionInfo
 * should call this function. See amtk_action_info_store_check_all_used().
 *
 * Since: 3.0
 */
void
amtk_action_info_mark_as_used (AmtkActionInfo *info)
{
	g_return_if_fail (info != NULL);

	info->used = TRUE;
}

/**
 * amtk_action_info_has_been_used:
 * @info: an #AmtkActionInfo.
 *
 * Returns whether @info has been used (for example by an #AmtkFactory
 * function). See also amtk_action_info_store_check_all_used().
 *
 * Returns: whether @info has been used.
 * Since: 3.0
 */
gboolean
amtk_action_info_has_been_used (const AmtkActionInfo *info)
{
	g_return_val_if_fail (info != NULL, FALSE);

	return info->used;
}
