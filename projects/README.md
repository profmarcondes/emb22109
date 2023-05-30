# Project directory
Use this directory to create per-project build directories. 

All files created here will be ignored by git.

Use the create.sh script to "automagically" create projects.

This script accepts one or two parameters, the first one is mandatory and is the
name of the project to be created. It checks if the directory already exists, and
in this case, doesn't allows it's creation. The second (optional) parameter is
the defconfig file to be used (without the _defconfig part of the name).

It already configures the project to use the external dir provided in this git repo,
so you can use defconfig from Buildroot and from the external dir (configs).

Examples:

   ./create.sh myProject
   
   ./create.sh myProject raspberrypi3
   
   ./create.sh myProject beaglebone
