from utils import validate_with_return_code, print_error, print_success, print_info, quit_daemon, clear_log, run_cmd, LOCK_FILE, LOG_FILE

'''
This file will test the compilation, running and options of the Matt_daemon program.
'''

def compilation_testing():
    location = "compilation_and_running_testing -> compilation_testing"
    if validate_with_return_code("make", 0) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def running_testing():
    location = "compilation_and_running_testing -> running_testing"
    if validate_with_return_code("./Matt_daemon", 0) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1
    
def re_running_testing():
    location = "compilation_and_running_testing -> re_running_testing"
    if validate_with_return_code("./Matt_daemon", 1) == True:
        print_success(location)
        return 0
    
    print_error(location)
    return 1

def client_options_ok():
    location = "compilation_and_running_testing -> client_options_ok"
    quit_daemon()
    cmd = "./Matt_daemon --client 3"
    if validate_with_return_code(cmd, 0) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def client_options_high():
    location = "compilation_and_running_testing -> client_options_high"
    quit_daemon()
    cmd = "./Matt_daemon --client 101"

    if validate_with_return_code(cmd, 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def client_options_high_high():
    location = "compilation_and_running_testing -> client_options_high_high"
    quit_daemon()
    cmd = "./Matt_daemon --client 100000001"

    if validate_with_return_code(cmd, 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def client_options_low():
    location = "compilation_and_running_testing -> client_options_low"
    quit_daemon()
    cmd = "./Matt_daemon --client -1"

    if validate_with_return_code(cmd, 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def client_options_low_low():
    location = "compilation_and_running_testing -> client_options_low_low"
    quit_daemon()
    cmd = "./Matt_daemon --client -100000001"

    if validate_with_return_code(cmd, 1) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def compilation_and_running_testing():
    print_info("=== compilation_and_running_testing ===")
    res = 0
    res += compilation_testing()
    res += running_testing()
    res += re_running_testing()
    res += client_options_ok()
    res += client_options_high()
    res += client_options_high_high()
    res += client_options_low()
    res += client_options_low_low()

    return res

if __name__ == "__main__":
    clear_log()
    quit_daemon()
    rm_lock_log_files()
    compilation_and_running_testing()