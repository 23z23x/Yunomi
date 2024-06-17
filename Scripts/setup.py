import os

def copyFile(src, dst):
    cmd = ""

    if os.name == 'nt':  # Windows
        cmd = f'copy "{src}" "{dst}"'
    else:  # Unix/Linux
        cmd = f'cp "{src}" "{dst}"'

    return cmd
 
# Copy File
os.system(copyFile('./premake5-GLFW.lua', '../Yunomi/vendor/GLFW/premake5.lua'))
os.system(copyFile('./stb_image.h', '../Yunomi/vendor'))
os.system(copyFile('./tiny_obj_loader.h', '../Yunomi/vendor'))