# Commands set

## build package

/cmd/build_package.sh \<package name\>

Lets you build a package specified by package name.

## build packages

/cmd/build_pckages.sh "\<packages to ignore>\"

Builds all packages present in the container, you can specifie names of packages you don't want to build.

## create package

/cmd/create_package.sh \<package name\>

Lets you create package with a specified name.

## create workspace

/cmd/create_workspace.sh

Lets you initialize ROS2 workspace.

## install package

/cmd/install_package.sh \<package name\>

It is a wrapper for **apt install** command.

## run cmd

/cmd/run_cmd.sh \<command\>

Lets you run any command in ROS 2 environment.