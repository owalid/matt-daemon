from compilation_and_running_testing import compilation_and_running_testing
from files_testing import files_testing
from server_testing import server_testing
from utils import quit_daemon, clear_log, rm_lock_log_files

if __name__ == "__main__":
    clear_log()
    quit_daemon()
    rm_lock_log_files()
    res = 0
    res += compilation_and_running_testing()
    res += files_testing()
    res += server_testing()
    rm_lock_log_files()
    exit(res)