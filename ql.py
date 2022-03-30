#!/usr/bin/env python

'''
This script processes extrinsic function library specification to
generate implementations of the querylibrary function in a number
languages.

Usage:
    ./ql.py {<specfile>}
    
If called without arguments, the script processes all *.spec files
in the current directory.

This script requires Python >= 3.0.
Additionally, we require Jinja (http://jinja.pocoo.org/docs/).
'''

import os;
import sys;
import collections;
from configparser import ConfigParser, ExtendedInterpolation
import jinja2;

APIVERSION = 2;

# some defaults
# NOTE: when changing, also update documentation in tri.spec (!)
DEFAULTS = {
    'languages'      : '',
    'libraryversion' : '1',
    'vendor'         : 'GAMS Development Corporation',
    'needlicense'    : '0',
    'notinequation'  : '0',  
    'zeroripple'     : '0',
    'derivative'     : 'continuous',
    'arguments'      : '',
    'exogenous'      : '',
    'endogenous'     : '__OTHER__',
    'maxderivative'  : '2'
    }

# maximal number of function arguments
MAXARGS = 20

# for debugging
SPECTEMPLATE = '''
[Library]
# for key in lib
{{key}} = {{lib[key]}}
# endfor
# for func in funcs
{##}
[{{func}}]
  # for key in funcs[func]
    # if key != 'args'
{{key}} = {{funcs[func][key]}}
    # endif
  # endfor
# endfor
'''

def processTemplate(configfile) :
    defaults = DEFAULTS.copy();
    # get default for stub from configfile, strip of path specification and .spec suffix
    configfilestub = os.path.basename(configfile);
    defaults['stub'] = configfilestub[:-5] if configfilestub.endswith('.spec') else configfilestub;
    
    # read the specification file
    config = ConfigParser(defaults, allow_no_value = True); #, interpolation=MyExtendedInterpolation())
    config.read(configfile);
    
    # we want to store the functions in an ordered dictionary
    # as the ConfigParser stores its sections in an ordered dictionary, this way
    # we can specify functions in the same order as given in the specification
    # (not necessary, but nice to have)
    funcs = collections.OrderedDict();
    
    # check for Library section
    if not config.has_section('Library') :
        print('Library section missing in specifications file', configfile, file=sys.stderr);
        return False;
    
    lib = dict(config.items('Library'));
    
    if 'extraexports' in lib :
        lib['extraexports'] = lib['extraexports'].split();
    lib['apiversion'] = APIVERSION;
        
    # process sections specifying functions
    for s in config.sections() :
        if s == 'Library' :
            continue;
        
        f = dict(config.items(s));
        funcs[s] = f;
        
        # split arguments at optional '|'
        argssplit = f['arguments'].split('|');
        if len(argssplit) > 2 :
            print('At most one | allowed in arguments specification for function', s, ':', f['arguments'], file=sys.stderr);
        
        # list of required arguments
        argsrequ = argssplit[0].split();
          
        # list of optional arguments
        if len(argssplit) == 2 :
            argsopt = argssplit[1].split();
        else :
            argsopt = [];
        
        # argmin and argmax
        f['argmin'] = len(argsrequ);
        f['argmax'] = f['argmin'] + len(argsopt);

        # store arguments as list, and fill up with empty strings until MAXARGS
        f['args'] = argsrequ + argsopt + [''] * (MAXARGS - f['argmax']);
        
        # get exogenous and endogenous operands
        exo = set(f['exogenous'].split());
        endo = set(f['endogenous'].split());
        exo.discard('|');
        endo.discard('|');

        # intersection needs to be empty
        assert len(exo & endo) == 0, 'Arguments cannot be both exogenuous and endogenuous (function ' + s + ')';
        
        # process __OTHER__ directive
        if exo == set(['__OTHER__']) :
            exo = set(argsrequ + argsopt) - endo;
        if endo == set(['__OTHER__']) :
            endo = set(argsrequ + argsopt) - exo;

        # union needs to cover arguments
        assert exo | endo >= set(argsrequ + argsopt), 'Arguments ' + str(set(argsrequ + argsopt) - exo - endo) + ' (function ' + s + ') not specified as exogenous or endogenous';
            
        # store bitmap: argumentname -> endogenous flag
        f['endogenous'] = {a : (1 if a in endo else 0) for a in f['args'] };
        f['endogenous'][''] = 0;
                
        # not needed anymore
        del f['arguments'];
        del f['exogenous'];
        
        # check that derivative is either continuous or discontinuous
        assert f['derivative'] in ['continuous', 'discontinuous'];
        # TODO further consistency checks

            
    env = jinja2.Environment(loader = jinja2.FileSystemLoader(os.path.dirname(__file__)),
                             line_statement_prefix = '#',
                             line_comment_prefix = '##');
    
    # debug
    #print env.from_string(SPECTEMPLATE).render(lib = lib, funcs = funcs);
    
    # allow to overwrite library name, usually not useful, but needed for C library fitfclib
    libname = lib['name'] if 'name' in lib else None;
    
    if 'C' in lib['languages'] :
        template = env.get_template('ql_template.c');
        lib['name'] = lib['stub'] + 'cclib' if libname is None else libname;
        outname = lib['name'] + 'ql.c';
        print('Generating', outname);
        outfile = open(outname, 'w');
        print(template.render(lib = lib, funcs = funcs), file=outfile);
        
        template = env.get_template('ql_template.def');
        lib['name'] = lib['stub'] + 'cclib' if libname is None else libname;
        outname = lib['name'] + '.def';
        print('Generating', outname);
        outfile = open(outname, 'w');
        print(template.render(lib = lib, funcs = funcs), file=outfile);
        
    if 'Delphi' in lib['languages'] :
        template = env.get_template('ql_template.dpr');
        lib['name'] = lib['stub'] + 'dclib' if libname is None else libname;
        outname = lib['name'] + 'ql.inc';
        print('Generating', outname);
        outfile = open(outname, 'w');
        print(template.render(lib = lib, funcs = funcs), file=outfile);

    if 'Fortran90' in lib['languages'] :
        template = env.get_template('ql_template.f90');
        lib['name'] = lib['stub'] + 'ifortlib' if libname is None else libname;
        outname = lib['name'] + 'ql.f90';
        print('Generating', outname);
        outfile = open(outname, 'w');
        print(template.render(lib = lib, funcs = funcs), file=outfile);
        
        template = env.get_template('ql_template.def');
        lib['name'] = lib['stub'] + 'fclib' if libname is None else libname;
        outname = lib['name'] + '.def';
        print('Generating', outname);
        outfile = open(outname, 'w');
        print(template.render(lib = lib, funcs = funcs), file=outfile);
        
    
if len(sys.argv) <= 1 :
    # if no arguments specified, call for each spec file we find in current directory
    for f in os.listdir('.') :
        if f.endswith('.spec') :
            processTemplate(f);
else :
    for f in sys.argv[1:] :
        processTemplate(f);
