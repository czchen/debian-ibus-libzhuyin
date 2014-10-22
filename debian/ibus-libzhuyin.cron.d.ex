#
# Regular cron jobs for the ibus-libzhuyin package
#
0 4	* * *	root	[ -x /usr/bin/ibus-libzhuyin_maintenance ] && /usr/bin/ibus-libzhuyin_maintenance
