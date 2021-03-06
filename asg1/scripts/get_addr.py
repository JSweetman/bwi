#!/usr/bin/env python

# Based on the turtlebot_addr.py script in the turtlebot_bringup package
# Original Authors: Tully Foote
# License: BSD
# Source: https://kforge.ros.org/turtlebot/turtlebot/raw-file/fb8354a98ac5/turtlebot_bringup/scripts/turtlebot_addr.py

import sys
import netifaces

if len(sys.argv) != 2:
    print >> sys.stderr, "ROS: Incorrect get_addr.py usage. USAGE get_addr.py <interface>. <interface> should be wlan0, eth0 etc."
    print >> sys.stderr, "ROS: Using eth0 as the default interface"
    interface = 'eth0'
else:
    interface = sys.argv[1]

for inf in netifaces.interfaces():
    if inf.startswith(interface):
        addrs = netifaces.ifaddresses(inf)
        if not netifaces.AF_INET in addrs:
            continue
        else:
            print addrs[netifaces.AF_INET][0]['addr']
            sys.exit(0)

print >> sys.stderr, "ROS: failed to determine IP address for interface %s. You won't be able to connect to this machine externally."%interface
print "127.0.0.1" #bind to loopback for now
sys.exit(1)
