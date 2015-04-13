import torrent_utils
import FileWrapperForPiece
from FilesForPiece import FilesForPiece
from utils import Profiler

def make_pieces_from_metadata(torrent, files):
    prf = Profiler()
    hashes = torrent_utils.get_hashes_from_metadata(torrent)
    piece_size = torrent_utils.get_piece_size(torrent)

    pieces = []
    stream_offset = 0
    
    cur_file_pos = 0
    for piece_index in range(len(hashes) - 1):
        piece = TPiece(piece_size, stream_offset, hashes[piece_index])
        cur_file_pos = piece.link_piece_and_files(files, cur_file_pos)   
        pieces.append(piece)
        
        stream_offset += piece_size
        
    final_piece_size = torrent_utils.get_final_piece_size(torrent)
    final_piece = TPiece(final_piece_size, stream_offset, hashes[-1])
    final_piece.link_piece_and_files(files, cur_file_pos)
    pieces.append(final_piece)
    
    return pieces

class TPiece:
    def __init__(self, size, stream_offset, phash):
        self.hash = phash
        self.size = size
        self.stream_offset = stream_offset
        self.files = FilesForPiece()                # файлы в которых присутствует текущий piece
        self.is_verified = False
        
    def get_end_offset(self):
        return self.stream_offset + self.size
    
    # по всем файлам ходить это не эффективно. файлы идут попорядку, так как они вчстроены в стриме ....
    def link_piece_and_files(self, all_files, cur_pos):
        if not all_files[cur_pos].is_intersect_with(self):
            cur_pos += 1
            
        while cur_pos < len(all_files):
            cur_file = all_files[cur_pos]
            if cur_file.is_intersect_with(self):
                wrap_file = FileWrapperForPiece.get_file_wrapper_for_piece(piece = self, file = cur_file)
                self.files.add(wrap_file)
                cur_pos += 1
            else: break
        
        if cur_pos == len(all_files) or not all_files[cur_pos].is_intersect_with(self): 
            cur_pos = cur_pos - 1
            
        return cur_pos

    def find_match(self):
        self.files.find_correct_file_combination(self.hash)
        self.is_verified = self.files.is_valid_combination
