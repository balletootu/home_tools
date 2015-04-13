import os
import shutil

def move_folder(src_path, dst_path):
    content = os.listdir(src_path)
    
    for item in content:
        shutil.move(os.path.join(src_path, item), dst_path)

def process_set(path):
    try:
        content = os.listdir(path)
        
        for item in content:
            if ('remium' in item) or ('tandard' in item):
                src_path = os.path.join(path, item)
                move_folder(src_path, path)
                shutil.rmtree(src_path)
    except :
        print('ERROR: processing set is fault for set: ', path)

def process_folder(path):
    sets_name = os.listdir(path)

    for item in sets_name:
        set_path = os.path.join(path, item)
        if os.path.isdir(set_path):
            process_set(set_path)
        
if __name__ == "__main__":
    cur_dir = os.getcwd()
    
    process_folder(cur_dir)