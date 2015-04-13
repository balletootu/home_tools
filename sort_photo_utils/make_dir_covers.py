import os

# заходит во все папки сетов и копирует файлы !cover.jpg в папку !covers\название_сета.jpg

COVERS_PATH = '!covers'
COVER_FILE_NAME = '!cover.jpg'

#рефакторинг сначало делается обход и ищутся все обложки (составляется список)
#затем функция копирует все обложки из списка (есть минус нужно будет парсить строку источник что бы знать во что переименовывать)

def copy_file(cur_dir, from_dir_name):
    from_dir = os.path.join(cur_dir, from_dir_name)
    if os.path.isdir(from_dir):        
        src = os.path.join(from_dir, COVER_FILE_NAME)        
        if os.path.exists(src):
            dst = os.path.join(cur_dir, os.path.join(COVERS_PATH, '%s.jpg' % from_dir_name))
            print(src, '=>', dst)
        
            try:    
                import shutil
                shutil.copy(src, dst)
            except :
                    print('Some errors')
        else:
            print(src,' doesn\'t exist!!!')
                
def fill_covers_dir(cur_dir):
    try:
        covers_path = os.path.join(cur_dir, COVERS_PATH)
        os.makedirs(covers_path)
    except:
        print('bla-bla-bla')
        
    names = os.listdir(path = cur_dir)
    
    for item in names:
        copy_file(cur_dir, item)
                
if __name__ == "__main__":
    cur_dir = os.getcwd()
    fill_covers_dir(cur_dir)