#!/usr/bin/env python
#***************************************************************************
#               ____   ___  ____   ____       _   __   __                  *
#              |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
#              | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
#              | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
#              |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
#                                                                          *
#  +-----------+---------------------------------------------------------+ *
#  |  File     | scp_data.py                                             | *
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
import os
import env

def ssh_command ( password, command):
    ssh_newkey = 'Are you sure you want to continue connecting'
    child = pexpect.spawn('scp  %s'%(command))
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
        child.sendline(password)
    if i==6: 
    	pass 
    child.expect(pexpect.EOF)       
    return child



f=open(env.DISRAY_hosts_path,'r')
lines=f.readlines()
f.close()
for line in lines:
	if(line[0]=="#"):
		continue
	break


child = ssh_command ( line.split()[2], ' -r /home/%s/DISRAY/data/ %s@%s:/home/%s/DISRAY/'%(os.getenv("USER"),line.split()[1],line.split()[0],line.split()[1]))

