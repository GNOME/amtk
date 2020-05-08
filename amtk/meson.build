amtk_public_headers = files(
  'amtk.h',
  'amtk-action-info.h',
  'amtk-action-info-central-store.h',
  'amtk-action-info-store.h',
  'amtk-action-map.h',
  'amtk-application-window.h',
  'amtk-factory.h',
  'amtk-gmenu.h',
  'amtk-init.h',
  'amtk-menu-item.h',
  'amtk-menu-shell.h',
  'amtk-shortcuts.h',
  'amtk-types.h',
  'amtk-utils.h',
)

amtk_sources = files(
  'amtk-action-info.c',
  'amtk-action-info-central-store.c',
  'amtk-action-info-store.c',
  'amtk-action-map.c',
  'amtk-application-window.c',
  'amtk-factory.c',
  'amtk-gmenu.c',
  'amtk-init.c',
  'amtk-menu-item.c',
  'amtk-menu-shell.c',
  'amtk-shortcuts.c',
  'amtk-utils.c',
)

amtk_enums = gnome.mkenums_simple('amtk-enum-types', sources : amtk_public_headers)

amtk_conf = configuration_data()
amtk_conf.set_quoted('GETTEXT_PACKAGE', 'amtk-5')
configure_file(output : 'config.h', configuration : amtk_conf)

cargs = [
  '-DDATADIR="@0@"'.format(get_option('prefix') / get_option('datadir')),
  '-DG_LOG_DOMAIN="Amtk"',
  '-DAMTK_COMPILATION',
]

if meson.get_compiler('c').get_argument_syntax() == 'msvc'
  cargs += '-D_AMTK_EXTERN=__declspec(dllexport)extern'
endif

deps = [
  dependency('gtk+-3.0', version : '>=3.22'),
]

amtk_incdir = include_directories('..')

libamtk = library('amtk', amtk_sources, amtk_enums,
  c_args : cargs,
  dependencies : deps,
  include_directories : amtk_incdir,
)

amtk_dep = declare_dependency(link_with : libamtk,
  include_directories : amtk_incdir)

pkg.generate(libamtk)
