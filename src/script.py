import os
import shutil
from distutils.dir_util import copy_tree

def move_file_to_downloads(file_name):
    current_path = os.path.join("src", file_name)
    print(f"Moving {file_name}")
    dest_path = os.path.join("C:\\Users\\rando\\Downloads", file_name)
    shutil.move(current_path, dest_path)

def main():
    EXECUTABLE_NAME = "Sokobus"

    if os.path.exists(os.path.join("src", EXECUTABLE_NAME + ".html")):
        os.rename(os.path.join("src", EXECUTABLE_NAME + ".html"), os.path.join("src", "index.html"))
        move_file_to_downloads("index.html")
        move_file_to_downloads(EXECUTABLE_NAME + ".data")
        move_file_to_downloads(EXECUTABLE_NAME + ".js")
        move_file_to_downloads(EXECUTABLE_NAME + ".wasm")
    
    copy_tree("assets", "C:\\Users\\rando\\Downloads\\assets")
    print("Copied assets folder")
    copy_tree("bin", "C:\\Users\\rando\\Downloads\\bin")
    print("Copied bin folder")
    
    input("Press enter to quit...")

if __name__ == '__main__':
    main()
