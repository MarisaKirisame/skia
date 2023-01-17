import os
import glob
from pathlib import Path
import subprocess

assert os.path.basename(os.getcwd()) == "zombie"

def shell(cmds):
    subprocess.run(cmds, shell=True, check=True)


shell("""gn gen ../out/Static --args='is_official_build=false extra_cflags_cc=["-frtti"]'""")

shell("ninja -C ../out/Static/ skottie_tool")

shell("rm -rf skottie_animation")

for x in os.listdir("skottie_code"):
    print(f"working on {x}...")
    path = Path("skottie_animation/"+os.path.splitext(x)[0])
    path.mkdir(parents=True, exist_ok=True)
    shell(f"../out/Static/skottie_tool -i skottie_code/{x} -w {path}")
