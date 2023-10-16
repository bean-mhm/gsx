This directory contains common files used by multiple projects. To avoid making extra copies, we
make symbolic links to the contents of this directory.

Here's an example of how you would create a symbolic link on Windows from within the include
directory of a project to another directory:
mklink /D ".\GLFW" "..\..\common\GLFW"

We also use symbolic links to use ECX's source code in the demo projects. Here's an example of how
you would create a symbolic link to ECX's source files on Windows from within the include
directory of a project:
mklink /D ".\ecx" "..\..\ecx\src"
