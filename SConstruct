#******************************************************************
#
# Institute for System Programming of the Russian Academy of Sciences
# Copyright (C) 2016 ISPRAS
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, Version 3.
#
# This program is distributed in the hope # that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU General Public License version 3 for more details.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

import os

cflags = ''
SConscript(os.environ['POK_PATH']+'/misc/SConscript', exports = 'cflags')

Import('env')
env['PARTITIONS'] = ['P1', 'P2', 'P3', 'P4']
env['XML'] = os.path.join(Dir('.').abspath, 'config.xml')
SConscript(env['POK_PATH']+'/misc/SConscript_base')

env.Clean('chpok', env['POK_PATH']+'/build/')
env.Clean('local', ['build/', [pdir+'/build' for pdir in env['PARTITIONS']]])

# EOF
