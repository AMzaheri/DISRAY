#!/usr/bin/env python
#***************************************************************************
#               ____   ___  ____   ____       _   __   __                  *
#              |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
#              | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
#              | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
#              |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
#                                                                          *
#  +-----------+---------------------------------------------------------+ *
#  |  File     | ssh.py                                                  | *
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



import pexpect
import getpass
import sys

def ssh_command (user, host, password, command):
    ssh_newkey = 'Are you sure you want to continue connecting'
    child = pexpect.spawn('ssh  %s@%s source ~/DISRAY/lib/env.sh ; ~/DISRAY/bin/DISRAYB & '%(user, host))
    i = child.expect([pexpect.TIMEOUT, ssh_newkey, 'password: ',"->","\:\~\$","ali@ali:",pexpect.EOF])


    if i == 0:
        print 'ERROR!'
        print 'SSH could not login. Here is what SSH said:'
        print child.before, child.after
        return None
    if i == 1:
        child.sendline ('yes')
        i = child.expect([pexpect.TIMEOUT, 'password: '])
        if i == 0:
            print 'ERROR!'
            print 'SSH could not login. Here is what SSH said:'
            print child.before, child.after
            return None       
        child.sendline(password)
    if i==2:
        if i == 0:
            print 'ERROR!'
            print 'SSH could not login. Here is what SSH said:'
            print child.before, child.after
            return None
        child.sendline(password)
    if i==6:
    	pass	
    child.expect(pexpect.EOF)
    return child


import env
f=open(env.DISRAY_hosts_path,'r')
lines=f.readlines()
f.close()
host_data=[]

HOSTs=[]
COMMENTs=[]
for line in lines:
	if(line[0]=="#"):
		COMMENTs += [line]
		continue
	host=line.split()
	try:
		host1=host[0].replace("\n","")
		user=host[1].replace("\n","")
		password=host[2].replace("\n","")
		print "-ok",host1,user,password
		child = ssh_command (user, host1, password, '/bin/ls -l')
		print "ok",host1,user,password
	except:
		print "err"




