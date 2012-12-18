#!/usr/bin/env python
#***************************************************************************
#               ____   ___  ____   ____       _   __   __                  *
#              |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
#              | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
#              | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
#              |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
#                                                                          *
#  +-----------+---------------------------------------------------------+ *
#  |  File     | tcp_server.py                                           | *
#  +-----------+---------------------------------------------------------+ *
#  |  Version  | 1                                                       | *
#  +-----------+---------------------------------------------------------+ *
#  |  Year     | 2012                                                    | *
#  +-----------+---------------------------------------------------------+ *
#  | Authors   | Afsaneh Mohammadzaheri <afsaneh.mohamadzaheri@gmail.com>| *
#  |           | Hossein Sadeghi <sadeghi@um.ac.ir>                      | *
#  |           | Keyvan Hosseini <k-hosseini@um.ac.ir>                   | *
#  |           | Mahdi Navazandeh <m.navazandeh@gmail.com>               | *
#  +-----------+---------------------------------------------------------+ *
#  |  Copyright (c) 2012 The DISRAY Authors. All rights reserved.        | *
#  +---------------------------------------------------------------------+ *
#  |  DISRAY : A Distributed Ray tracing by Map-Reduce.                  | *
#  |  Make sure the input(source) is set.                                | *
#  +------------------------------------------------------------------ --+ *
#                                                                          *
#***************************************************************************



import socket
import env


TCP_IP = ''#'127.0.0.1'
TCP_PORT = 34606
BUFFER_SIZE = 10

f=open(env.DISRAY_count_path)
str_count=f.read()
f.close()


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

for i in range(int(str_count)):
	conn, addr = s.accept()
	print 'Connection address:', addr
	data = conn.recv(BUFFER_SIZE)
	if not data: break
	print "received data:", len(data)
	conn.send(data)
	conn.close()
s.close()
print "end"
