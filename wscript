#!/usr/bin/env python
import os
import shutil
from waflib.extras import autowaf as autowaf

# Version of this package (even if built as a child)
MDA_VERSION = '1.0.0'

# Variables for 'waf dist'
APPNAME = 'mda-lv2'
VERSION = MDA_VERSION

# Mandatory variables
top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx')
    autowaf.set_options(opt)

def configure(conf):
    conf.load('compiler_cxx')
    autowaf.configure(conf)
    conf.line_just = 23
    autowaf.display_header('MDA.lv2 Configuration')

    autowaf.check_pkg(conf, 'lv2', atleast_version='1.0.0', uselib_store='LV2')

    conf.env.append_unique('CFLAGS', '-std=c99')

    # Set env['pluginlib_PATTERN']
    pat = conf.env['cxxshlib_PATTERN']
    if pat[0:3] == 'lib':
        pat = pat[3:]
    conf.env['pluginlib_PATTERN'] = pat
    conf.env['pluginlib_EXT'] = pat[pat.rfind('.'):]

    autowaf.display_msg(conf, "LV2 bundle directory",
                        conf.env['LV2DIR'])
    print('')

def build_plugin(bld, lang, bundle, name, source, cflags=[], libs=[]):
    # Build plugin library
    penv = bld.env.derive()
    penv['cshlib_PATTERN']   = bld.env['pluginlib_PATTERN']
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = '%s %sshlib' % (lang,lang))
    obj.env          = penv
    obj.source       = source + ['lvz/wrapper.cpp']
    obj.includes     = [ '.', './lvz', './src' ]
    obj.name         = name
    obj.target       = os.path.join(bundle, name)
    if cflags != []:
        obj.cxxflags = cflags
    if libs != []:
        autowaf.use_lib(bld, obj, libs)
    obj.install_path = '${LV2DIR}/' + bundle
    obj.uselib = ['LV2']

    # Install data file
    data_file = '%s.ttl' % name
    bld.install_files('${LV2DIR}/' + bundle, os.path.join(bundle, data_file))

def build(bld):
    # Copy data files to build bundle (build/mda.lv2)
    def do_copy(task):
        src = task.inputs[0].abspath()
        tgt = task.outputs[0].abspath()
        return shutil.copy(src, tgt)
        #cmd = 'cp %s %s' % (src, tgt)
        #return task.exec_command(cmd)

    for i in bld.path.ant_glob('mda.lv2/[A-Z]*.ttl'):
        bld(rule   = do_copy,
            source = i,
            target = bld.path.get_bld().make_node('mda.lv2/%s' % i),
            install_path = '${LV2DIR}/mda.lv2')

    bld(features = 'subst',
        source   = 'mda.lv2/manifest.ttl.in',
        target   = bld.path.get_bld().make_node('mda.lv2/manifest.ttl'),
        LIB_EXT  = bld.env['pluginlib_EXT'],
        install_path = '${LV2DIR}/mda.lv2')

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
#            Looplex

    # Build plugin libraries
    for i in plugins:
        build_plugin(bld, 'cxx', 'mda.lv2', i, ['src/mda%s.cpp' % i],
                     ['-DPLUGIN_CLASS=mda%s' % i,
                      '-DURI_PREFIX=\"http://drobilla.net/plugins/mda/\"',
                      '-DPLUGIN_URI_SUFFIX="%s"' % i,
                      '-DPLUGIN_HEADER="src/mda%s.h"' % i])
