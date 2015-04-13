import os

# копируются коверы лежащие отдельно в папки с сетами
# начальные условия: ковер должен называтся так же как и папка с сетом
# и в текущей папке лежат папки с сетами и папка !covers  с обложками

def copy_covers_to_set(path):
    names = os.listdir(path = path)
    
    covers_path = os.path.join(path, '!covers')
    
    for item in names:
        set_path = os.path.join(path, item)
        if os.path.isdir(set_path) and  (item != '!covers') :            
            src = os.path.join(covers_path, '{0}.jpg'.format(item))
            dst = os.path.join(set_path, '!cover.jpg')
            try:    
                import shutil
                shutil.copy(src, dst)
                print(src, '=>', dst)
            except :
                    print('Some errors')

def find_cover_name(covers_name, set_name):
    set_name_comp = set_name.split('-')
    date = '%s-%s-%s' %(set_name_comp[0], set_name_comp[1], set_name_comp[2])
    for item in covers_name:
        if ('.jpg' in item) and (date in item) and (set_name_comp[3] in item) :
            return item
    return ''
                    
def fj_copy_covers_to_set(path):
    sets_name = os.listdir(path)

    covers_path = os.path.join(path, '!covers')
    covers_name = os.listdir(covers_path)
    
    for item in sets_name:
        set_path = os.path.join(path, item)
        if os.path.isdir(set_path) and (item != '!covers'):            
            src = os.path.join(covers_path, find_cover_name(covers_name, item))
            dst = os.path.join(set_path, '!cover.jpg')
            try:    
                import shutil
                shutil.copy(src, dst)
                print(src, '=>', dst)
            except :
                    print('Some errors')

                
if __name__ == "__main__":
    cur_dir = os.getcwd()
    
#    copy_covers_to_set(cur_dir)
    fj_copy_covers_to_set(cur_dir)
#     names = os.listdir(path = cur_dir)
#     for item in names:
#         if os.path.isdir(item):
#             sp_str1 = item.split(' HQ Photoset with ')
#             sp_str2 = sp_str1[0].split(' ')
#             new_name = sp_str2[0] + ' - ' + sp_str1[1] + ' - ' + ' '.join(sp_str2[1:])
#             print(new_name)
            