from setuptools import *

setup(
    name='SudokuSolverLibrary',
    packages=find_packages(),
    #packages=['integration'],
    ext_modules=[Extension('_SudokuSolverLib', ['swig.i'])],
        #swig_opts=['-I../include'])],
)