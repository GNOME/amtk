amtk_public_headers = [
  'amtk.h',
  'amtk-action-info-central-store.h',
  'amtk-action-info.h',
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
]

amtk_public_c_files = [
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
]

headers_install_dir = get_option('includedir') / 'amtk-@0@/amtk/'.format(AMTK_API_VERSION)
install_headers(
  amtk_public_headers,
  install_dir: headers_install_dir
)

amtk_enum_types = GNOME.mkenums_simple(
  'amtk-enum-types',
  decorator: '_AMTK_EXTERN',
  header_prefix: '#include <amtk/amtk-types.h>',
  sources: amtk_public_headers,
  install_header: true,
  install_dir: headers_install_dir
)

amtk_static_lib_c_args = [ '-DAMTK_COMPILATION' ]
if meson.get_compiler('c').get_argument_syntax() == 'msvc'
  amtk_static_lib_c_args += '-D_AMTK_EXTERN=__declspec(dllexport) extern'
endif

amtk_static_lib = static_library(
  'amtk-static',
  [amtk_public_c_files,
   amtk_enum_types],
  pic: true, # amtk_static_lib is linked in a shared library.
  include_directories: ROOT_INCLUDE_DIR,
  dependencies: AMTK_DEPS,
  c_args: amtk_static_lib_c_args
)

# For unit tests, to be able to test private functions.
AMTK_STATIC_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: amtk_static_lib,
  sources: amtk_enum_types[1],
  dependencies: AMTK_DEPS
)

amtk_lib_link_args = []
amtk_lib_link_depends = []

if meson.get_compiler('c').get_argument_syntax() != 'msvc'
  symbol_map = meson.current_source_dir() / 'symbol.map'
  amtk_lib_link_args = '-Wl,--version-script,' + symbol_map
  amtk_lib_link_depends = symbol_map
endif

amtk_lib = library(
  'amtk-@0@'.format(AMTK_API_VERSION),
  dependencies: AMTK_DEPS,
  link_args: amtk_lib_link_args,
  link_depends: amtk_lib_link_depends,
  # link_whole is not supported with MSVC, so we use extract_all_objects().
  objects: amtk_static_lib.extract_all_objects(),
  version: AMTK_LT_VERSION,
  install: true
)

AMTK_LIB_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: amtk_lib,
  sources: amtk_enum_types[1],
  dependencies: AMTK_DEPS
)

PKG_CONFIG.generate(amtk_lib,
  filebase: 'amtk-@0@'.format(AMTK_API_VERSION),
  name: 'Amtk',
  description: 'Actions, Menus and Toolbars Kit',
  subdirs: 'amtk-@0@'.format(AMTK_API_VERSION),
  libraries: AMTK_DEPS,
)

# Introspection
gir = find_program('g-ir-scanner', required : get_option('introspection'))
build_gir = gir.found() and (not meson.is_cross_build() or get_option('introspection').enabled())

if build_gir
  GNOME.generate_gir(
    amtk_lib,
    export_packages: 'amtk-@0@'.format(AMTK_API_VERSION),
    header: 'amtk/amtk.h',
    identifier_prefix: 'Amtk',
    include_directories: ROOT_INCLUDE_DIR,
    includes: ['Gtk-3.0'],
    install: true,
    namespace: 'Amtk',
    nsversion: AMTK_API_VERSION,
    sources: [
      amtk_public_headers,
      amtk_public_c_files,
      amtk_enum_types
    ],
    # Support for deps being built as subprojects:
    dependencies: AMTK_DEPS,
  )
endif
