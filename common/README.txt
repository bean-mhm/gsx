This directory contains common files used by multiple projects. To avoid making extra copies, we
make symbolic links to the contents of this directory.

Here's an example of how you would create a symbolic link on Windows from within the include
directory of a project:
mklink /D ".\glm" "..\..\common\glm"

We also use symbolic links to use TEF's source code in the demo projects. Here's an example of how
you would create a symbolic link to TEF's source files on Windows from within the include
directory of a project:
mklink /D ".\tef" "..\..\tef\src"
