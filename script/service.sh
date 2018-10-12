#!/bin/sh

# You have to SET the service installation directory here
SERVICE_DIR="YOUR_SERVICE_INSTALLATION_PATH"
SERVICE_USER="USER_YOU_WANT_SERVICE_RUN_WITH"
LOG_DIR="../log"
APP_NAME="serviceTest"

usage() {
	echo "Usage: `basename $0`: <start|stop|restart|status>"
	exit 1
}

start() {
	status
	if [ $PID -gt 0 ]
	then
		echo "Service daemon was already started. PID: $PID"
		return $PID
	fi
	echo "Starting Service daemon..."
	cd "$SERVICE_DIR/bin"
	# something like
	# su $SERVICE_USER -c "cd \"$SERVICE_DIR/bin\"; /usr/bin/nohup ./start.sh 1>$LOG_DIR/service.log 2>$LOG_DIR/service.err &"
}

stop() {
	status
	if [ $PID -eq 0 ]
	then
		echo "Service daemon is already not running"
		return 0
	fi
	echo "Stopping Service daemon..."
	cd "$SERVICE_DIR/bin"
	# something like
    # su $SERVICE_USER -c "cd \"$SERVICE_DIR/bin\"; /usr/bin/nohup ./shutdown.sh 1>>$LOG_DIR/service.log 2>>$LOG_DIR/service.err &"
}

status() {
	PID=` ps auxw | grep $APP_NAME | grep -v grep | awk '{print $2}'`
	if [ "x$PID" = "x" ]
	then
		PID=0
	fi

	# if PID is greater than 0 then Service is running, else it is not
	return $PID
}

if [ "x$1" = "xstart" ]
then
	start
	exit 0
fi

if [ "x$1" = "xstop" ]
then
	stop
	exit 0
fi

if [ "x$1" = "xrestart" ]
then
	stop
	start
	exit 0
fi

if [ "x$1" = "xstatus" ]
then
	status
	if [ $PID -gt 0 ]
	then
		echo "Service daemon is running with PID: $PID"
		exit 0
	else
		echo "Service daemon is NOT running"
		exit 3
	fi
fi

usage