#!/usr/bin/env python
import os
import re
import shutil
import waflib.extras.autowaf as autowaf

MDA_VERSION = '1.1.0'

# Mandatory waf variables
APPNAME = 'MDA'        # Package name for waf dist
VERSION = MDA_VERSION  # Package version for waf dist
top     = '.'          # Source directory
out     = 'build'      # Build directory

def options(opt):
    opt.load('compiler_cxx')
    autowaf.set_options(opt)

def configure(conf):
    conf.load('compiler_cxx')
    autowaf.configure(conf)
    conf.line_just = 23
    autowaf.display_header('MDA.lv2 Configuration')

    autowaf.check_pkg(conf, 'lv2', atleast_version='1.0.0', uselib_store='LV2')

    autowaf.display_msg(conf, "LV2 bundle directory",
                        conf.env.LV2DIR)
    print('')

def build(bld):
    # Make a pattern for shared objects without the 'lib' prefix
    module_pat = re.sub('^lib', '', bld.env.cxxshlib_PATTERN)
    module_ext = module_pat[module_pat.rfind('.'):]

    plugins = '''
            Ambience
            Bandisto
            BeatBox
            Combo
            DX10
            DeEss
            Degrade
            Delay
            Detune
            Dither
            DubDelay
            Dynamics
            EPiano
            Image
            JX10
            Leslie
            Limiter
            Loudness
            MultiBand
            Overdrive
            Piano
            RePsycho
            RezFilter
            RingMod
            RoundPan
            Shepard
            Splitter
            Stereo
            SubSynth
            TalkBox
            TestTone
            ThruZero
            Tracker
            Transient
            VocInput
            Vocoder
    '''.split()

    for p in plugins:
        bundle = 'mod-mda-%s.lv2' % p

        # Build manifest by substitution
        bld(features     = 'subst',
            source       = 'bundles/%s/manifest.ttl.in' % bundle,
            target       = bld.path.get_bld().make_node('%s/manifest.ttl' % bundle),
            LIB_EXT      = module_ext,
            install_path = '${LV2DIR}/%s' % bundle)

        # Build plugin library
        obj = bld(features     = 'cxx cxxshlib',
                  source       = ['src/mda%s.cpp' % p, 'lvz/wrapper.cpp'],
                  includes     = ['.', './lvz', './src'],
                  name         = p,
                  target       = os.path.join(bundle, p),
                  install_path = '${LV2DIR}/' + bundle,
                  uselib       = ['LV2'],
                  defines      = ['PLUGIN_CLASS=mda%s' % p,
                                  'URI_PREFIX="http://moddevices.com/plugins/mda/"',
                                  'PLUGIN_URI_SUFFIX="%s"' % p,
                                  'PLUGIN_HEADER="src/mda%s.h"' % p])
        obj.env.cxxshlib_PATTERN = module_pat
        obj.env.SHLIB_MARKER = ''
        obj.env.STLIB_MARKER = ''

        # Set extra files for install
        for i in bld.path.ant_glob('bundles/%s/*.ttl' % bundle):
            bld(features     = 'subst',
                is_copy      = True,
                source       = i,
                target       = bld.path.get_bld().make_node('%s/%s' % (bundle, i)),
                install_path = '${LV2DIR}/%s' % bundle)

        # Install data file
        bld.install_files('${LV2DIR}/' + bundle, os.path.join(bundle, p + '.ttl'))
