# Commands set

## build package

``` bash
/cmd/build_package.sh \<package name\>
```

Lets you build a package specified by package name.

## build packages

``` bash
/cmd/build_pckages.sh "\<packages to ignore>\"
```

Builds all packages present in the container, you can specifie names of packages you don't want to build.

## create package
``` bash
/cmd/create_package.sh \<package name\>
```
Lets you create package with a specified name.

## create workspace
``` bash
/cmd/create_workspace.sh
```
Lets you initialize ROS2 workspace.

## install package
``` bash
/cmd/install_package.sh \<package name\>
```
It is a wrapper for **apt install** command.

## run cmd
``` bash
/cmd/run_cmd.sh \<command\>
```
Lets you run any command in ROS 2 environment.

## update contaienr packages
``` bash
/cmd/upgrade.sh
```