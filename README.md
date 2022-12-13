# Z80_ICE

STM32 CUBE IDE has a default project folder: stmcube.
The Z80 project and the libZ80 are placed in this folder.
This folder also has folders 'lib' and 'include' for linking with libraries.

To use in the STM32 CUBE IDE, make a new project.

In the project's folder, add Src and Inc folders.
Add the file Z80.ioc.
Copy c files in Src, header files in Inc.
In Core/Src, replace the generated main.c with the one provided here.
In Core/Inc, replace the generated main.h with the one provided here.

In the IDE, import the project by pointing to the project's folder.

If using libZ80 as a library:
  Add the modified libZ80
  In the IDE, import the project by pointing to the project's folder
  build the project
  copy libz80.a in the folder 'lib'.
  copy z80.h and z80_struct.h in the folder 'include'.
  
  projects using any library should be configured to point to the 'include' and 'lib' folders
  
Integrated with Z80_ICE
  copy the source files (*.c) in 'Src', header files (*.h) in 'Inc'
  
