#!/usr/bin/env python
#***************************************************************************
#               ____   ___  ____   ____       _   __   __                  *
#              |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
#              | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
#              | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
#              |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
#                                                                          *
#  +-----------+---------------------------------------------------------+ *
#  |  File     | stdin-ter.py                                            | *
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



import sys
import os
import env

IP_defualt=""
USER_NAME_defualt=""
PASS_defualt=""


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
        if(line[0]=="\n"):
                continue
        HOSTs += [line.split()]
try:
        IP_defualt = HOSTs[0][0]
        USER_NAME_defualt = HOSTs[0][1]
        PASS_defualt = HOSTs[0][2]
except:
        pass

#print IP_defualt
#print USER_NAME_defualt
#print PASS_defualt

if IP_defualt == "":
        USER_NAME_defualt="node"
        PASS_defualt="pass"
if USER_NAME_defualt == "":
        USER_NAME_defualt="node"
if PASS_defualt == "":
        PASS_defualt="pass"


def Client_INFO():

        global HOSTs
        flag_edit=False
        while ( 1):
                IP = raw_input("\nIP or [enter] : ")
                if IP == "":
                        return

                for i in range(len(HOSTs)):
                        if IP == HOSTs[i][0]:
                                flag_edit = True
                                break
                if flag_edit:
                        USER_NAME = raw_input("Enter (%s) user-name : [%s] " %(IP, HOSTs[i][1]) )
                        if USER_NAME == "":
                                USER_NAME = HOSTs[i][1]


                        PASS = raw_input("Enter (%s@%s) passwoard : [%s] " %(USER_NAME, IP, HOSTs[i][2]) )
                        if PASS == "":
                                PASS = HOSTs[i][2]

                        HOSTs[i][1]=USER_NAME
                        HOSTs[i][2]=PASS
                        flag_edit=False
                        
                else:
                        USER_NAME = raw_input("Enter (%s) user-name : [%s] " %(IP, USER_NAME_defualt) )
                        if USER_NAME == "":
                                USER_NAME = USER_NAME_defualt


                        PASS = raw_input("Enter (%s@%s) passwoard : [%s] " %(USER_NAME, IP, PASS_defualt) )
                        if PASS == "":
                                PASS = PASS_defualt	
                        
                        HOSTs += [[IP,USER_NAME,PASS]]


                f=open(env.DISRAY_hosts_path,"w")	
                f.write("#====IP==== 	===username==	===pass===\n")
                for host in HOSTs:
                        IP = host[0].replace("\n","")
                        USER_NAME = host[1].replace("\n","")
                        PASS = host[2].replace("\n","")
                        f.write("%s	%s	        %s\n"%(IP,USER_NAME,PASS))
                for comment in COMMENTs:
                        if comment == "#====IP==== 	===username==	===pass===\n":
                                continue
                        f.write(comment)
                f.close()
                


def Server_INFO():
        #====================server================================
        try:
                server_config=open(env.DISRAY_server_cfg_path,"r")
        except:
                pass


        server_config=open(env.DISRAY_server_cfg_path,"w")
        server_ip=raw_input("\nServer ip or Hostname : ")
        server_Username=raw_input("Enter username : ")
        server_pass=raw_input("Enter Server pass : ")
        server_config.write("#====IP==== 	===username==	===pass===\n")
        server_config.write("%s	%s	        %s\n"%(server_ip,server_Username,server_pass))
        server_config.close()
        ser=open(env.DISRAY_server_ip_path,"w")
        ser.write(server_ip)
        ser.close()

def Client_print():
        f=open(env.DISRAY_hosts_path,'r')
        s=f.read()
        s="   #  "+s
        s=s.replace('\n','\n   #  ')                
        print 
        print s

def Menu_print():
        print
        print "A) Add client/s"
        print "S) Server info."
        print "P) show clients"
        print "Q) Quit setup"
        cmd=raw_input("\ncommands [A]:")
        while ( 1 ):
                if cmd == "Q" or cmd == "q" or cmd == "Quit" or cmd == "QUIT" or cmd == "quit":
                        print "Good By"
                        break
                if cmd == "A" or cmd == "a" or cmd == "Add" or cmd == "ADD" or cmd == "add" or  cmd.replace(" ","")=="":
                        Client_INFO()
                if cmd == "S" or cmd == "s" or cmd == "Server" or cmd == "SERVER" or cmd == "server":
                        Server_INFO()

                if cmd=="p":
                        Client_print()
                print
                print "A) Add client/s"
                print "S) Server info."
                print "P) show clients"
                print "Q) Quit setup"
                cmd=raw_input("\ncommands [A]:")


def Final():
        f=open(env.DISRAY_hosts_ssh_path,"w")
        for host in HOSTs:
                IP = host[0].replace("\n","")
                USER_NAME = host[1].replace("\n","")
                PASS = host[2].replace("\n","")
                f.write("%s@%s\n"%(USER_NAME,IP))	
        f.close()

        f=open(env.DISRAY_count_path,"w")
        f.write(str(len(HOSTs)))
        f.close()

        f=open(env.DISRAY_env_sh_path,"w")
        f.write("N_COUNT=%s\n"% (str(len(HOSTs) ) ) )
        f.close()

        import scp
        import ssh_start
        _id=0
        for host in HOSTs:
                try:
                        IP = host[0].replace("\n","")
                        USER_NAME = host[1].replace("\n","")
                        PASS = host[2].replace("\n","")
                        child = scp.ssh_command ( PASS, ' -r /home/%s/DISRAY %s@%s:.'%(os.getenv("USERNAME"),USER_NAME,IP))
                        _ssh=ssh_start.ssh_command(USER_NAME,IP,PASS,"echo %s >~/DISRAY/config/id"%(_id))
                        _id+=1
                        print 'ssh OK  for [%s@%s].'%(USER_NAME , IP)
                except:
                        _id+=1
                        print 'ssh error in IP=%s , USERNAME=%s , PASS=%s'%(IP,USER_NAME,PASS)


Menu_print()
Final()
