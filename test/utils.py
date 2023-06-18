import subprocess
from colorama import Fore, Style

LOCK_FILE = "/var/lock/matt_daemon.lock"
LOG_FILE = "/var/log/matt_daemon/matt_daemon.log"

def validate_with_return_code(cmd, return_code_expected):
    '''
    This function will run the command and check if the return code is the same as the expected one.

    cmd: the command to run <str>
    return_code_expected: the expected return code <int>
    '''
    try:
        res = subprocess.run(cmd.split(' '), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return_code = res.returncode
        if return_code == return_code_expected:
            return True
        return False
    except:
        return False

def validate_with_fn(cmd, fn):
    '''
    This function will run the command and pass the result to the function.
    The function should return True if the result is valid, False otherwise.

    cmd: the command to run <str>
    fn: the function to validate the result <function>
    '''
    try:
        res = subprocess.check_output(cmd, shell=True)
        return fn(res), res
    except subprocess.CalledProcessError as e:
        print(e)
        return False, None

def run_cmd(cmd):
    '''
    This function will run the command and return the result.

    cmd: the command to run <str>
    '''
    try:
        res = subprocess.check_output(cmd, shell=True)
        return res
    except:
        return None

def print_error(error_msg):
    print(Fore.RED, f"[FAIL] {error_msg}", Style.RESET_ALL)

def print_success(success_msg):
    print(Fore.GREEN, f"[SUCCESS] {success_msg}", Style.RESET_ALL)

def print_info(info_msg):
    print(Fore.BLUE, f"[INFO] {info_msg}", Style.RESET_ALL)

def quit_daemon():
    cmd = "./scripts/send_content.sh quit"
    subprocess.run(cmd.split(' '))

def clear_log():
    cmd = "./scripts/send_content.sh clear"
    subprocess.run(cmd.split(' '))

def run_daemon():
    cmd = "./Matt_daemon"
    subprocess.run(cmd.split(' '))

def rm_lock_log_files():
    run_cmd(f"rm -rf {LOCK_FILE}")
    run_cmd(f"rm -rf {LOG_FILE}")