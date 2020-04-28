#!/usr/bin/python
#
# Utility script to generate .pc files for GLib
# for Visual Studio builds, to be used for
# building introspection files

# Author: Fan, Chun-wei
# Date: March 10, 2016

import os
import sys

from replace import replace_multi
from pc_base import BasePCItems

def main(argv):
    amtk_api_ver = '5'
    base_pc = BasePCItems()

    base_pc.setup(argv)
    pkg_replace_items = {'@AMTK_API_VERSION@': amtk_api_ver,
                         '@PACKAGE_VERSION@': base_pc.version,
                         '@AX_PACKAGE_REQUIRES@': 'glib-2.0 >= 2.52, gtk+-3.0 >= 3.22',
                         '@AX_PACKAGE_REQUIRES_PRIVATE@': ''}

    pkg_replace_items.update(base_pc.base_replace_items)

    # Generate amtk-$(amtk_api_ver).pc
    replace_multi(base_pc.top_srcdir + '/amtk.pc.in',
                  base_pc.srcdir + '/amtk-' + amtk_api_ver + '.pc',
                  pkg_replace_items)

if __name__ == '__main__':
    sys.exit(main(sys.argv))
