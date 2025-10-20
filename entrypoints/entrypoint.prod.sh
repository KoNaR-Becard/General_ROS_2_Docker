echo "Hello Prod"

MicroXRCEAgent udp4 -p 8888 &

/cmd/run_cmd.sh ros2 launch /app/launch/launch.sample.py