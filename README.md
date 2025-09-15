# Bee3

Small engine like pack of dynamic libraries. 
Built from ground up with minmal use of standard library headers, mainly focusing on use of operating system headers.
Still linux and windows compatible.

### Pretty much, heavy work in progress.

## Goal

Main goal of this project is a playground for learning and experimenting with new ideas.
I really want to avoid use of standard libraries like thread, string etc and use system implementations instead.
In future this code might be a good fundaiton for a real application or cool indie game.
It's a hobby project that I work on during my available free time.

## Build

### Linux

Run provided Build.sh scrpit in root directory of this repository.
Use `--help` option to list available build options.

To compile a project from Samples directory:
```
./Build.sh --sample `PROJECT_NAME`
```

To compile a project from Projects directory:
```
./Build.sh `PROJECT_NAME`
```

To complie a project from custom directory:
```
./Build.sh --customtargetpath ./../MyProjects `PROJECT_NAME`
```

### Windows

Use provided Build.bat script.

Currently work in progress.



