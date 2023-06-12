import subprocess

def run_and_validate(cmd, return_code_expected):
    print("Running...")
    result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return_code = result.returncode
    if return_code == return_code_expected:
        print("running successful!")
