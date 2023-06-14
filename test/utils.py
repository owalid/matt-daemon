import subprocess
from colorama import Fore, Style

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
    except:
        return False, None

def print_error(error_msg):
    print(Fore.RED, f"[FAIL] {error_msg}", Style.RESET_ALL)

def print_success(success_msg):
    print(Fore.GREEN, f"[SUCCESS] {success_msg}", Style.RESET_ALL)

def print_info(info_msg):
    print(Fore.BLUE, f"[INFO] {info_msg}", Style.RESET_ALL)

def quit_daemon():
    cmd = "./send_command.sh quit"
    subprocess.run(cmd.split(' '))

def clear_log():
    cmd = "./send_command.sh clear"
    subprocess.run(cmd.split(' '))