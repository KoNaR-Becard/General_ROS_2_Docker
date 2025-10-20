#!/bin/bash

echo "Installing dependencies"

apt update

apt install gettext -y

cat '/dep/packages.txt' | envsubst | xargs apt -y install

rosdep update

pip3 install --upgrade pip 

rosdep install --from-paths /app --ignore-src  -r -y -q

find /app -name "dependencies.txt" -exec pip3 install -r {} \;