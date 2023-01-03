import os
import subprocess
import sys

assert os.path.basename(os.getcwd()) == 'zombie'

def build():
    subprocess.run("rm -rf cpu/* gl/* zombie_gl/* diff/* zombie_diff/*", shell=True, check=True)
    subprocess.run("ninja -C ../out/Static zombie skdiff", shell=True, check=True)

def normal():
    build()

    subprocess.run("../out/Static/zombie", shell=True, check=True)
    subprocess.run("../out/Static/skdiff cpu gl diff", shell=True, check=True)
    subprocess.run("../out/Static/skdiff gl zombie_gl zombie_diff", shell=True, check=True)

def debug():
    build()

    subprocess.run("gdb -ex=r --args ../out/Static/zombie debug", shell=True, check=True)

args = sys.argv[1:]
if args == []:
    normal()
elif args == ["debug"]:
    debug()
else:
    print(args)
