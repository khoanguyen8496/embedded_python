#!/usr/bin/python2

### originated from the ibm developerswork blog
import sysconfig
import string, sys
import glob
import re
source_re = re.compile(r'(.+?)\.c', re.IGNORECASE);
configopts = {}
maketemplate = """
CXX=g++

PYLIB=%(pythonlib)s

PYINC=-I%(pythoninc)s

LIBS=%(pylibs)s

OPTS=%(pyopt)s

DEPS=%(deps)s

PROGRAMS=%(programs)s

all: $(PROGRAMS) $(DEPS)
"""
configopts['pythonlib'] = '-L' + sysconfig.get_config_var('LIBPL') \
        + ' ' + \
        '-lpython' + str(sysconfig.get_config_var('py_version_short'))
configopts['pythoninc'] = ''
configopts['pylibs'] = sysconfig.get_config_var('LINKFORSHARED') + ' '
# configopts['pylibs'] = ''
for dir in string.split(sysconfig.get_config_var('INCLDIRSTOMAKE')):
    configopts['pythoninc'] += '-I%s ' % (dir,)
for dir in string.split(sysconfig.get_config_var('LIBDIR')):
    configopts['pylibs'] += '-L%s ' % (dir,)
configopts['pylibs'] += sysconfig.get_config_var('MODLIBS') \
        + ' ' + \
        sysconfig.get_config_var('LIBS') \
        + ' ' + \
        sysconfig.get_config_var('SYSLIBS')
# configopts['pyopt'] = sysconfig.get_config_var('OPT')
configopts['pyopt'] = '-DNDEBUG -g -fwrapv -O2 -Wall'
source_obj = []
for source_file in glob.iglob("*.cpp"):
    source_obj.append(re.sub(r'(.+?)\.cpp', r'\1.o', source_file))
configopts['deps'] = ' '.join(source_obj)
targets = ''
arg = sys.argv[1]
targets += arg + ' '
configopts['programs'] = targets
print maketemplate % configopts
# for arg in sys.argv[1]:
print "%s: $(DEPS)\n\t$(CXX) $^ $(LIBS) $(PYLIB) -o $@ \n" \
        % (arg)
print "%.o: %.cpp\n\t$(CXX) -o $@ $^ -c $(PYINC) $(OPTS)\n"
print "clean:\n\trm -f $(PROGRAMS) *.o *.pyc core"
