from compilation_and_running_testing import compilation_and_running_testing
from utils import quit_daemon, clear_log

if __name__ == "__main__":
    clear_log()
    quit_daemon()
    res = 0
    res += compilation_and_running_testing()
    exit(res)