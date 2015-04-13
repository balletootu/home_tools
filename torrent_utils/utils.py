
# функция возвращает список элементов равных x
# key используется в случае, если нужно сравнить не с самим элементом в списке а с какой то из его составляющих
# например key = lambda x: x['len'] 
def get_range(li, x, key = lambda x: x):
    size_li = len(li)
    i, j = 0, size_li - 1
    while i < j:
        m = int ((i + j) / 2)
        if x > key(li[m]):
            i = m + 1
        else:
            j = m
            
    res = []
    if key(li[i]) != x:
        return res
    
    # расширяем границы вниз i указывает на первое вхождение
    while True:
        if ((i - 1) < 0) or (key(li[i - 1]) != x) : break
        i -= 1
        
    # расширяем границы вверх j указывает на следующий эл. после последнего вхождения
    while True:
        if (j >= size_li) or (key(li[j]) != x) : break
        j += 1
        
    while i != j:
        res.append(li[i])
        i += 1

    return res

# возвращает список всех файлов находящихся по заданному пути (со всеми вложениями)
# каждый элемент списка - словарь из элементов path, name, len
# список отсортированный по размеру файла        
def get_files_list(path):
    import os
    files = []
    for (dirname, subshere, fileshere) in os.walk(path):
        for fname in fileshere:
            cur_path = os.path.join(dirname, fname)
            file = {'name':fname, 'len':os.path.getsize(cur_path), 'path':dirname}
            files.append(file)
    
    files.sort(key=lambda x: x['len'])
    return files

from datetime import datetime
class Profiler:
    def __init__(self):
        self.time = datetime.now()
        
    def start(self):
        self.time = datetime.now()
                
    def log(self, msg):
        now = datetime.now()
        print(msg, now - self.time)
        self.time = now
