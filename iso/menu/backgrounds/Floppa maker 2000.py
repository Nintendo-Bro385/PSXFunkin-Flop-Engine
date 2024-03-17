import shutil
import subprocess
import os
import random

number = 1
floppa=int(input("how many vram and palette files do you want "))
for counter in range(floppa):
    new_file_name = "week" + str(number) + ".png.txt"
    original = r'week0.png.txt'
    shutil.copy(original, new_file_name)

    subprocess.call((new_file_name), shell=True)
    print("Creating idle",number,".png.txt")
    number=number+1
