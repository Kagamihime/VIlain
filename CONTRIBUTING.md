# Contributing to VIlain

This project will focus on a few aspects for the quality of its source code:

- The source code must be well-formated (ensured by coala).
- The project must build successfully (ensured by Travis CI).
- The code coverage must be as high as possible (measured by Codecov).

Hence no pull requests will be merged to the master branch without passing all checks and being reviewed.

## GitHub workflow

Do not work on anything without being assigned to an issue. If what you want to do isn't amongst the existing issues, please create a new one.

Once you are assigned to an issue, you will create a new branch to work on it. The name of this branch should start with `your-name/`.

Commits should be atomic: you should commit each change you will do into a separate commit (i.e. when you modify several files to solve an issue or when you do several modifications that can be logically splitted). It will make things like bisecting much more efficient. You may be asked to squash you commits if you did too many ones, or split them if you should have done more.

Please, put the reference to the issue you are solving in the first comment of your PR (for example, by writting `Closes [URL TO THE ISSUE]` or `Fixes [URL TO THE ISSUE]` on the first or last line when you submit your PR) if it wasn't automatically done by GitHub.

You may need to rebase your branch before being able to merge it. If you need help for rebasing or squashing your commits, you can take a look at https://github.com/servo/servo/wiki/Beginner's-guide-to-rebasing-and-squashing.

## Commit message guidelines

Example of a good commit:

```
setup.py: Change bears' entrypoint

This entrypoint ensures that coala discovers
the bears correctly.
It helps not writing more functions inside
``coalib`` for this.

Closes https://github.com/coala/coala/issues/5861
```

A commit message consists of 3 parts:

- shortlog
- commit body
- issue reference

### Shortlog

- Maximum of 50 characters.

- Should describe the _change_ - the action being done in the commit.

- Should have a tag and a short description separated by a colon (`:`)

  - _Tag_: the file or module being modified.

  - _Short description_

    - Starts with a capital letter.
    - Written in imperative present tense (i.e. `Add something`, **not** `Adds something`, `Adding something` or `Added something`).
    - No trailing period.

### Commit body

- Maximum of 72 chars excluding newline for _each_ line.
- Not mandatory - but helps explain what you're doing.
- First person should _not_ be used here.

### Issue reference

Example:

```
Closes https://github.com/Kagamihime/VIlain/issues/2
```

- Should use the `Fixes` keyword if your commit fixes a bug or a typo, or `Closes` otherwise.
- Should use full URL to the issue.
- There should be a single space between the `Fixes` or `Closes` and the URL.

## Coding style

The linux coding style will be used, except that the source files will be indented using 4 spaces instead of 8. See: <https://www.kernel.org/doc/html/v4.10/process/coding-style.html>

Note that the formatting can be automatically done when running coala locally.

## Naming convention

- Macros', constants' and structs' names should be written in `SCREAMING_SNAKE_CASE`.

- Other variables' and functions' names should be written in `snake_case`.

- Avoid using `CamelCase` as it is rarely used in C.

## Testing

### coala

Please, run coala locally to check your code before submitting a PR.

To run coala, you can easily use the script `coala.sh`:

- `./coala.sh help` will print help for this script.
- `./coala.sh install` will install coala in the project's root.
- `./coala.sh run` will run coala's checks.
- `./coala.sh clean` will remove the directory made by `./coala.sh install`.

<!-- ### Check -->
