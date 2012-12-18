#!/usr/bin/env python
#***************************************************************************
#               ____   ___  ____   ____       _   __   __                  *
#              |  _ \ |_ _|/ ___| |  _ \     / \  \ \ / /                  *
#              | | | | | | \___ \ | |_) |   / _ \  \ V /                   *
#              | |_| | | |  ___) ||  _ <   / ___ \  | |                    *
#              |____/ |___||____/ |_| \_\ /_/   \_\ |_|   VERSION 1.       *
#                                                                          *
#  +-----------+---------------------------------------------------------+ *
#  |  File     | env.py                                                  | *
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



import os
home_dir=os.getenv("HOME")
DISRAY_HOME_path=home_dir+"/DISRAY"
DISRAY_server_cfg_path=home_dir+"/DISRAY/config/server.cfg"
DISRAY_server_ip_path=home_dir+"/DISRAY/config/server.ip"
DISRAY_hosts_path=home_dir+"/DISRAY/config/DISRAY.hosts"
DISRAY_count_path=home_dir+"/DISRAY/config/count"
DISRAY_id_path=home_dir+"/DISRAY/config/id"
DISRAY_hosts_ssh_path=home_dir+"/DISRAY/config/.hosts.ssh"
DISRAY_env_sh_path=home_dir+"/DISRAY/lib/env.sh"
DISRAY_env_py_path=home_dir+"/DISRAY/lib/env.sh"
DISRAY_data_path=home_dir+"/DISRAY/data"
DISRAY_log_path=home_dir+"/DISRAY/log"
