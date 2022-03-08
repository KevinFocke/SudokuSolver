Ver 1 - The basics: 

x Create a program that solves simple sudokus inefficiently. No premature optimization!

x Can read in space-delimited text files.

x Sudoku struct includes 2D Matrix representation. 

Ver 2 - Solid foundations & backtracking: 

x Make extendible with different algorithms.

x Refactor functions to make struct sudoku the primary data representation.

x Implement backtracking algorithm

- Support Sudoku's up to 36 x 36.


Ver 3 - Testing + Documentation:
- Start developing on a linux environment (New WSL window)
- Track issues on Github (possible from terminal?)
- feat - constrainedAlgo, looks at most constrained first.
- Add command line recognition of flags, filename=mytext.txt (<argp.h> library, compiled using gcc, part of glibc library)
- Add automated test cases via CI (Jenkins, Travis CI, Buddy)
- Check for memory leaks (valgrind)
- Add automated security testing (WhiteSource Bolt, Snyk)



Ver 4:

- readFile, prevent octal interpretation of code. (013 being interpreted as 1 * 8 + 3 * 1 = 11)

- Extending Python with C (https://docs.python.org/2/extending/extending.html). CAPI ? ctypes & CFFI interface? 
https://cffi.readthedocs.io/en/latest/overview.html

(possible to pass what the Python OCR reads to standard input?)

- Make picture of sudoku, translates into matrix

- Make it possible to automatically fetch sudokus from a website.


Ver 5:

- Create GUI for sudoku solver (QT?)
- Support Snake Matrix (using boxmatrix record)
- Add hint system
- Emscripten, using clang LLVM compiler https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm ?