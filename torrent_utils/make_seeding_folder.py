# попытка создать из существующего контента раздачу на основе .torrent файла
# попервой будем делать через копирование имеющегося контента в спец папку

import sys
import bencode
import os
import shutil
from utils import get_files_list
from TFile import matching_files_by_size
from TPiece import make_pieces_from_metadata
from utils import Profiler

# приблизительная структура *.torrent файла
# 'comment'
# 'publisher'
# 'encoding'
# 'publisher-url'
# 'announce'
# 'creation date'
# 'created by'
# 'info'
#     'name'              # имя раздачи (имя корневй папки раздачи для мультифайловой раздачи или имя файла для однофайловой раздачи)
#     'piece lenght'      
#     'pieces'            # бинарная строка с хашами кусков
#     'files'                # в однофайловой раздаче этого раздела нет
#         'path'          # список запчастей пути
#         'length'        # размер файла в байтах
        
def make_seeding_folder(torrent_name, src_dir, dst_dir):
    with open(torrent_name, 'rb') as fh:
        torrent_bytes = fh.read()
            
    torrent = bencode.decode(torrent_bytes)
    tor_name = torrent['info']['name']
     
    print(tor_name)
    
    dst_dir_1 = os.path.join(dst_dir, tor_name)
    os.makedirs(name = dst_dir_1, exist_ok = True)

    prf = Profiler()
    files = get_files_list(src_dir)
    prf.log('get_files_list() delay:')
    files = matching_files_by_size(torrent, files)
    prf.log('matching_files_by_size() delay:')
    
    pieces = make_pieces_from_metadata(torrent, files)
    prf.log('make_pieces_from_metadata() delay:')
    
    for idx in range(len(pieces)):
        pieces[idx].find_match()
#        print('Checked ', idx, ' piece from ', len(pieces))
    prf.log('find_matches() delay:')
        
    for f in files:
        if f.is_matched():
            dst_file = os.path.join(dst_dir, f.get_torrent_name())
            src_file = f.get_matched_name()
            
            dst_path = os.path.join(dst_dir, f.get_path_from_torrent())
            if not os.path.exists(dst_path):
                os.makedirs(name = dst_path, exist_ok = True)
            
#            print(src_file, '=>', dst_file)
            shutil.copy(src_file, dst_file)

    prf.log('copy files delay:')

if __name__ == "__main__":
    file_name, src_dir, dst_dir = 'test.torrent', r'.\src', r'.\dst'
    if len(sys.argv) != 4:
        print("Usage: make_seeding_folder.py torrentfile.torrent src_dir dst_dir")
    else:
        file_name, src_dir, dst_dir = sys.argv[1], sys.argv[2], sys.argv[3]
    
    make_seeding_folder(file_name, src_dir, dst_dir)
