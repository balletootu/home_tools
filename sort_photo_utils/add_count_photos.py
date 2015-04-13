import os

# заходит во все папки текущей директории и считает количество .jpg файлов исключая !cover.jpg
# результат записывается в файл 1.txt каждая строка это имя_папки (xкол-во_фоток)

RENAMES_FILE_NAME = '1.txt'
COUNT_OF_TMP_FILES = 1      #количество файлов не входящих в сет коверы и т.д.

def get_photo_count(dir_path):
    if os.path.isdir(dir_path):
        
        count = 0
        names = os.listdir(path = dir_path)
        for item in names:
            if ('.jpg' in item) and item != '!cover.jpg':
                count += 1
        return count
    
    return 0
    
def gen_renames_file(dir_path):
    names = os.listdir(path = dir_path)
    
    renames = []
    for item in names:
        processing_dir = os.path.join(dir_path, item)
        if os.path.isdir(processing_dir):
            renames.append('{0} (x{1})'.format(item, get_photo_count(processing_dir)))
            
    f = open(RENAMES_FILE_NAME, 'wb')  
    for i in renames:
        f.write((i + '\r\n').encode('utf-8') );
    f.close()


if __name__ == "__main__":
    cur_dir = '.'

    gen_renames_file(cur_dir)