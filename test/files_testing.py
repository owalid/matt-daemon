from utils import validate_with_return_code, print_error, print_success, print_info, quit_daemon, clear_log, run_daemon, run_cmd, validate_with_fn, LOCK_FILE, LOG_FILE, rm_lock_log_files

'''
This file will test the files that are used and created by the Matt_daemon program.
'''

# lock_file
def lock_file_exist():
    quit_daemon()
    run_daemon()
    location = "files_testing -> lock_file_exist"
    res, log = validate_with_fn(f"ls {LOCK_FILE}", lambda val: len(val) > 0)
    if res == True:
        print_success(location)
        return 0
    print(log)
    print_error(location)
    return 1

def delete_lock_file():
    quit_daemon()
    run_daemon()
    location = "files_testing -> delete_lock_file"
    run_cmd(f"rm {LOCK_FILE}")
    clear_log()
    run_daemon()
    res, log = validate_with_fn(f"sleep 0.2; cat {LOG_FILE} | grep 'Quitting. Hard failure : Unable to bind to any address.'", lambda val: len(val) > 0)
    if res == True:
        print_success(location)
        return 0
    print(log)
    print_error(location)
    return 1

def change_lock_file_to_directory():
    quit_daemon()
    rm_lock_log_files()
    run_cmd(f"mkdir {LOCK_FILE}")
    location = "files_testing -> change_lock_file_to_directory"
    if validate_with_return_code("./Matt_daemon", 1) == True:
        print_success(location)
        rm_lock_log_files()
        return 0
    rm_lock_log_files()
    print_error(location)
    return 1
    
# log_file
def log_file_exist():
    quit_daemon()
    run_daemon()
    location = "files_testing -> lock_file_exist"
    if validate_with_return_code(f"ls {LOG_FILE}", 0) == True:
        print_success(location)
        return 0
    print_error(location)
    return 1

def delete_log_file():
    quit_daemon()
    run_daemon()
    location = "files_testing -> delete_log_file"
    run_cmd(f"rm {LOG_FILE}")
    clear_log()
    if validate_with_return_code("./Matt_daemon", 1) == True:
        print_success(location)
        rm_lock_log_files()
        return 0
    rm_lock_log_files()
    print_error(location)
    return 1

def change_log_file_to_directory():
    quit_daemon()
    rm_lock_log_files()
    run_cmd(f"mkdir {LOG_FILE}")
    location = "files_testing -> change_log_file_to_directory"
    if validate_with_return_code("./Matt_daemon", 1) == True:
        print_success(location)
        run_cmd(f"rm {LOG_FILE}")
        return 0
    run_cmd(f"rm {LOG_FILE}")
    print_error(location)
    return 1

def files_testing():
    print_info("=== files_testing ===")
    clear_log()
    quit_daemon()
    rm_lock_log_files()
    res = 0
    res += lock_file_exist()
    res += delete_lock_file()
    res += change_lock_file_to_directory()
    res += log_file_exist()
    res += delete_log_file()
    res += change_log_file_to_directory()
    return res

if __name__ == "__main__":
    clear_log()
    quit_daemon()
    rm_lock_log_files()
    files_testing()