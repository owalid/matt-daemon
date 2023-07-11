from utils import validate_with_return_code, print_error, print_success, print_info, quit_daemon, clear_log, run_daemon, run_cmd, validate_with_fn, LOCK_FILE, LOG_FILE, rm_lock_log_files
from subprocess import Popen, PIPE, STDOUT
import pty

'''
This file will test the server, port binding and options of the Matt_daemon program.
'''

def server_quit_daemon():
    location = "server_testing -> server_quit_daemon"
    run_daemon()
    quit_daemon()
    res, log = validate_with_fn(f"cat {LOG_FILE}", lambda val: len(val) > 0)
    if res == True:
        print_success(location)
        return 0
    print(log)
    print_error(location)
    return 1

def server_listen_good_port():
    location = "server_testing -> server_listen_good_port"
    quit_daemon()
    run_daemon()
    res, log = validate_with_fn(f"lsof -i -P -n | grep LISTEN | grep 4242", lambda val: len(val) > 0)
    if res == True:
        print_success(location)
        return 0
    print(log)
    print_error(location)
    return 1

def server_not_listen_after_quit():
    location = "server_testing -> server_not_listen_after_quit"
    quit_daemon()
    run_daemon()
    quit_daemon()
    if validate_with_return_code(f"lsof -i -P -n | grep LISTEN | grep 4242", 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def pid_not_exist_after_quit():
    location = "server_testing -> pid_not_exist_after_quit"
    quit_daemon()
    run_daemon()
    quit_daemon()

    if validate_with_return_code(f"ps aux | grep Matt_daemon | grep -v grep", 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def server_cant_listen_on_port():
    location = "server_testing -> server_cant_listen_on_port"
    quit_daemon()
    clear_log()
    _, slave = pty.openpty()
    Popen("nc -lvp 4242&", shell=True, stdin=PIPE, stdout=slave, stderr=slave, close_fds=True)
    run_daemon()
    res, log = validate_with_fn(f"sleep 1; cat {LOG_FILE} | grep 'Unable to bind to any address.'", lambda val: len(val) > 0)
    # kill nc process
    run_cmd("kill $(ps aux | grep nc | grep -v grep | awk '{print $2}')")
    if res == True:
        print_success(location)
        return 0
    print(log)
    print_error(location)
    return 1

def server_testing():
    print_info("=== server_testing ===")
    clear_log()
    quit_daemon()
    rm_lock_log_files()
    res = 0
    res += server_quit_daemon()
    res += server_listen_good_port()
    res += server_not_listen_after_quit()
    res += pid_not_exist_after_quit()
    res += server_cant_listen_on_port()
    rm_lock_log_files()
    return res

if __name__ == "__main__":
    server_testing()
