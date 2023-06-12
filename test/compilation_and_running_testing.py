from utils import run_and_validate

def compilation_testing():
    print("Compiling...")
    run_and_validate("make", 0)

def running_testing():
    print("Running...")
    run_and_validate("./Matt_deamon", 0)

def re_running_testing():
    print("Re-running...")
    run_and_validate("./Matt_deamon", 0)

def compilation_and_running_testing():
    compilation_testing()
    running_testing()
    re_running_testing()

if __name__ == "__main__":
    compilation_and_running_testing()