
# подсчет sha1 хэша
# особенности: создается хэш-файл для каждой папки находящейся в текущей (1 уровень)
# расчет происходит на всю глубину папок

BUF_SIZE =  2 * 2**10

def sha1OfFile(filepath):
    import hashlib
    sha = hashlib.sha1()
    with open(filepath, 'rb') as f:
        while True:
            block = f.read(BUF_SIZE)
            if not block: break
            sha.update(block)
        return sha.hexdigest()
    
#рекурсивный расчет хешей для папки dir_path со всеми вложениями
def hash_dir(dir_path, hashes, cur_level = ''):
    names = os.listdir(path = dir_path)
    for item in names:
        cur_path = os.path.join(dir_path, item)
        loc_level = os.path.join(cur_level, item)
        if os.path.isfile(cur_path):
            hash_for_file = '{0} *{1}'.format(sha1OfFile(cur_path), loc_level)
            hashes.append(hash_for_file)
            print(hash_for_file)
        else:
            hash_dir(cur_path, hashes, loc_level)

#создание файла хешей для папки dir_path
def make_hashes_file_for_dir(dir_path, file_name):
    if os.path.isdir(dir_path):
        hashes = []
        hash_dir(dir_path, hashes)
        
        #записать в файл хеши из hashes
        fiel_path = os.path.join(dir_path, file_name)
        f = open(fiel_path, 'w')
        
        for i in hashes:
            f.write(i + '\n');
            
        f.close()
                   
def hash_year_dir(dir_path):
    names = os.listdir(path = dir_path)

    for item in names:
        hash_dir = os.path.join(dir_path, item)
        if os.path.isdir(hash_dir):
            make_hashes_file_for_dir(hash_dir, '{0}.sha'.format(item))
                            
if __name__ == "__main__":
    import os
    cur_dir = '.' #os.getcwd()

    hash_year_dir(cur_dir)
