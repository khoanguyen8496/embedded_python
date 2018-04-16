#!/usr/bin/python2
import distutils.sysconfig
import string, sys
import glob
import re
source_re = re.compile(r'(.+?)\.c', re.IGNORECASE);
configopts = {}
maketemplate = """
PYLIB=%(pythonlib)s
PYINC=-I%(pythoninc)s
LIBS=%(pylibs)s
OPTS=%(pyopt)s
DEPS=%(deps)s
PROGRAMS=%(programs)s
all: $(PROGRAMS) $(DEPS)
"""
configopts['pythonlib'] = distutils.sysconfig.get_config_var('LIBPL') \
        + '/' + \
        distutils.sysconfig.get_config_var('LIBRARY')
configopts['pythoninc'] = ''
configopts['pylibs'] = distutils.sysconfig.get_config_var('LINKFORSHARED') + ' '
# configopts['pylibs'] = ''
for dir in string.split(distutils.sysconfig.get_config_var('INCLDIRSTOMAKE')):
    configopts['pythoninc'] += '-I%s ' % (dir,)
for dir in string.split(distutils.sysconfig.get_config_var('LIBDIR')):
    configopts['pylibs'] += '-L%s ' % (dir,)
configopts['pylibs'] += distutils.sysconfig.get_config_var('MODLIBS') \
        + ' ' + \
        distutils.sysconfig.get_config_var('LIBS') \
        + ' ' + \
        distutils.sysconfig.get_config_var('SYSLIBS')
configopts['pyopt'] = distutils.sysconfig.get_config_var('OPT')
source_obj = []
for source_file in glob.iglob("*.c"):
    source_obj.append(re.sub(r'(.+?)\.c', r'\1.o', source_file))
configopts['deps'] = ' '.join(source_obj)
targets = ''


arg = sys.argv[1]
targets += arg + ' '
configopts['programs'] = targets
print maketemplate % configopts
# for arg in sys.argv[1]:
print "%s: $(DEPS)\n\tgcc $(DEPS) $(LIBS) $(PYLIB) -o %s" \
        % (arg, arg)
print "%.o: %.c\n\tgcc -o $@ $^ -c $(PYINC) $(OPTS)"
print "clean:\n\trm -f $(PROGRAMS) *.o *.pyc core"
