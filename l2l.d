#!/usr/bin/env bash

source functions_personal

# WARNING : for the moment, the path has to be provide as absolute to the daemon.
# As it will execute the command in a 'fresh' environment, it will not have access to environment variable or variable set here before running the 'daemonize' command.

SLEEP_TIME_MINIMUM_BETWEEN_EACH_DAEMON_LOOP=10

# Run a the l2l as a daemon
# This daemon will re-run the command at the maximum each X seconds
# Or more if the command took time to execute.
daemonize -c '/home/incognito/Desktop/dev/l2l/l2l' -s $SLEEP_TIME_MINIMUM_BETWEEN_EACH_DAEMON_LOOP --silent

exit 0
