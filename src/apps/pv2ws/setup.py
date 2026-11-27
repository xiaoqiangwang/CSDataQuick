#!/usr/bin/env python

"""
setup.py file for pv2ws
"""
from setuptools import setup


# load module from file
def load_module(name, location):
    import importlib.util
    spec = importlib.util.spec_from_file_location(name, location)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


long_description = open('README.rst').read()


_version = load_module('_version', 'pv2ws/_version.py')

setup(name='pv2ws',
      version=_version.__version__,
      description="""Process Variables over Websocket""",
      long_description=long_description,
      author="Xiaoqiang Wang",
      author_email="xiaoqiang.wang@psi.ch",
      url="https://github.com/xiaoqiangwang/CSDataQuick",
      packages=["pv2ws"],
      entry_points={
          'console_scripts': ['pv2ws=pv2ws.aiows:main']
      },
      install_requires=['CaChannel', 'aiohttp', 'cbor2'],
      license="BSD",
      platforms=["Windows", "Linux", "Mac OS X"],
      classifiers=['Development Status :: 4 - Beta',
                   'Environment :: Console',
                   'Intended Audience :: Developers',
                   'License :: OSI Approved :: BSD License',
                   'Programming Language :: Python :: 3',
                   ],
      )
