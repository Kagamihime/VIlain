# VIlain

A repository for the project of the course "Conduite de Projet - 2017/2018" <https://upsilon.cc/~zack/teaching/1718/cproj/>

Students: Gaëlle MARAIS, Eisha CHEN-YEN-SU, David VO.

# Dependencies

In order to use coala, you have to install `python3`,`indent` and `cppcheck` first.

In order to run the tests, you have to install `check`.

In order to launch and build VIlain you need `libncurses`.

# Compilation and execution

To compile the project use the command:
```
make
```

To launch the program use the command:
```
./build/vilain
```
To launch the program and open a specific file use the command:
```
./build/vilain file_name
```
To execute the tests use the commands:
```
cd tests
make test
```

# Why GitHub instead of GitLab?
We want to use Travis CI to test our project at every Pull Request to be sure
that they are working before merging them.

Travis checks if the PR pass all the checks and if the code is properly indented.
In this purpose, we also use coala to automatically correct the indentation and
the space consistency of the code.

See more about Travis: <https://travis-ci.org/>

See more about coala: <https://coala.io/>

# Development log
To understand how we worked on this project, please consult `CONTRIBUTING.md `.

We did not write a development log because we used the issues that are made for this purpose. In the issues, you can see how we worked week after week and you can read our discussions about development choices and work distribution.

# Work distribution

**Eisha CHEN-YEN-SU**
- The set up of the project with Coala and Travis and the file `CONTRIBUTING.md`.
- Parts of the Makefile.
- All the implementation of the module `buffer.c` and its header.
- The tests for the module `buffer.c`.


**Gaëlle MARAIS**
- The file `README.md` and parts of the Makefile.
- The module `cursor.c` and its header.
- All the user interface : the module `ui.c` and its header.
- Parts of the module `settings.c`.
- The module `main.c` and the default configuration with `ui.cfg`.

**David VO**
- The first UML before starting the implementation.
- Parts of the module `settings.c`.
- The module `io_text` and its header.
- The tests for the modules `io_text.c` and `settings.c` (Hopefully)
