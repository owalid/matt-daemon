make
./Matt_daemon 
ls
./send_command.sh lol
ls -al
exit
make
./Matt_daemon 
rm -rf /var/lock/matt_daemon.lock 
./scripts/send_content.sh lol
./Matt_daemon 
./scripts/send_content.sh clear
./scripts/send_content.sh quit
echo '' > /var/log/matt_daemon/matt_daemon.log
mkdir -p /var/lock/matt_daemon.lock
./Matt_daemon 
rm -rf /var/lock/matt_daemon.lock/
./Matt_daemon 
rm -rf /var/log/matt_daemon/matt_daemon.log 
./Matt_daemon 
./scripts/send_content.sh lol
rm -rf /var/log/matt_daemon/matt_daemon.log
./scripts/send_content.sh lol
./scripts/send_content.sh quit
rm -rf /var/log/matt_daemon/matt_daemon.log 
mkdir -p /var/log/matt_daemon/matt_daemon.log
./Matt_daemon 
make
exit
