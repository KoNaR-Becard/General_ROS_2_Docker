#!/bin/bash

clear

arg=$1


if [ "$arg" = "start" ]; then

docker compose -f compose_sim.yml up --remove-orphans

elif [ "$arg" = "start_ros" ]; then

docker compose -f compose_sim.yml exec -t gazebo /run_sim.sh

elif [ "$arg" = "stop" ]; then

docker compose -f compose_sim.yml stop -t 3600

elif [ "$arg" = "clean" ]; then

rm -R workspace/build
rm -R workspace/install

elif [ "$arg" = "compile" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/build_packages.sh

elif [ "$arg" = "purge" ]; then

docker compose -f compose_sim.yml down

elif [ "$arg" = "rebuild" ]; then

docker compose -f compose_sim.yml build --no-cache

elif [ "$arg" = "build" ]; then

docker compose -f compose_sim.yml build

elif [ "$arg" = "cmd" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/run_cmd.sh "${@:2}"

elif [ "$arg" = "logs" ]; then

docker compose -f compose_sim.yml logs gazebo

elif [ "$arg" = "exec" ]; then

docker compose -f compose_sim.yml exec gazebo "${@:2}"

elif [ "$arg" = "run" ]; then

docker compose -f compose_sim.yml run gazebo /cmd/run_cmd.sh "${@:2}"

elif [ "$arg" = "debug" ]; then

docker compose -f compose_sim.yml exec gazebo bash

elif [ "$arg" = "topics" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/run_cmd.sh ros2 topic list -t

elif [ "$arg" = "echo" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/run_cmd.sh ros2 topic echo "${@:2}"

elif [ "$arg" = "info" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/run_cmd.sh ros2 topic info "${@:2}"

elif [ "$arg" = "publish" ]; then

docker compose -f compose_sim.yml exec gazebo /cmd/run_cmd.sh ros2 topic pub --once "${@:2}"

else

echo "start - start a container"
echo "stop - stop a container"
echo "compile - build all packages"
echo "purge - remove containers"
echo "rebuild - build image without caches"
echo "build - build image"
echo "cmd - run command in working container, in ros2 workspace"
echo "logs - show container logs"
echo "exec - general run command in working container"
echo "run - run command in container"
echo "debug - enter containter shell"
echo "topics - list topics"
echo "echo - show messages published on topic"
echo "info - show topic info"
echo "publish - publish message into specific topic"

fi
