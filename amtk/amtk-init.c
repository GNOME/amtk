/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017-2020 - Sébastien Wilmet <swilmet@gnome.org>
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
#include "amtk-init.h"
#include <glib/gi18n-lib.h>
#include "amtk-action-info-central-store.h"

static gchar *
get_locale_directory (void)
{
#ifdef G_OS_WIN32
	gchar *base_dir;
	gchar *locale_dir;

	base_dir = g_win32_get_package_installation_directory_of_module (NULL);
	locale_dir = g_build_filename (base_dir, "share", "locale", NULL);
	g_free (base_dir);

	return locale_dir;
#else
	return g_build_filename (DATADIR, "locale", NULL);
#endif
}

/**
 * amtk_init:
 *
 * Initializes the Amtk library (e.g. for the internationalization).
 *
 * This function can be called several times, but is meant to be called at the
 * beginning of main(), before any other Amtk function call.
 *
 * Since: 3.0
 */
void
amtk_init (void)
{
	static gboolean done = FALSE;

	if (!done)
	{
		gchar *locale_dir;

		locale_dir = get_locale_directory ();
		bindtextdomain (GETTEXT_PACKAGE, locale_dir);
		g_free (locale_dir);

		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

		done = TRUE;
	}
}

/**
 * amtk_finalize:
 *
 * Free the resources allocated by Amtk. For example it unrefs the singleton
 * objects.
 *
 * It is not mandatory to call this function, it's just to be friendlier to
 * memory debugging tools. This function is meant to be called at the end of
 * main(). It can be called several times.
 *
 * Since: 3.0
 */

/* Another way is to use a DSO destructor, see gconstructor.h in GLib.
 *
 * The advantage of calling amtk_finalize() at the end of main() is that
 * gobject-list [1] correctly reports that all Amtk* objects have been finalized
 * when quitting the application. On the other hand a DSO destructor runs after
 * the gobject-list's last output, so it's much less convenient, see:
 * https://gitlab.gnome.org/GNOME/gtksourceview/commit/e761de9c2bee90c232875bbc41e6e73e1f63e145
 *
 * [1] A tool for debugging the lifetime of GObjects:
 * https://github.com/danni/gobject-list
 */
void
amtk_finalize (void)
{
	static gboolean done = FALSE;

	/* Unref the singleton only once, even if this function is called
	 * multiple times, to see if a reference is not released correctly.
	 * Normally the singleton have a ref count of 1. If for some reason the
	 * ref count is increased somewhere, it needs to be decreased
	 * accordingly, at the right place.
	 */
	if (!done)
	{
		_amtk_action_info_central_store_unref_singleton ();
		done = TRUE;
	}
}
