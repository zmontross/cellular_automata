## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
##
## ZDM
## To get this working I needed to do the following:
## 1. Download/install the MinGW-m64 (distinct from normal MinGW which is the older variant, 32bit-only). I installed both the 32bit (i686, aka i386 online) and 64bit (x86_64) variants TO THE SAME DIR AS MY C/CPP WORK.
## 2. Download/extract (alternatively clone) the raylib 3.0 repository.
## 3. Create project directory.
## 4. Copy/paste the contents of /<raylib_repo_dir>/projects/VSCode/* into the project directory.
## 5. Within the newly-copied files update ALL *Windows* references to compiler location and raylib location.
## 6. !!! Navigate to the raylib/src directory (the source code directory) and run "make all"
## 7. Specifically call the 32bit installation of "mingw32-make.exe" which installed to "/<mingw-m64_dir>/mingw32/bin/mingw32-make.exe". Both 32 and 64 versions have this same exe.
## 8. !!! In the project-specific Makefile remove the reference to '$(RAYLIB_PATH)/src/raylib.rc.data' from the CFLAGS Makefile variable.
## 9. Repeat the "make all" step in the project directory. "game.exe" will be placed in the project directory.
## 
## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 