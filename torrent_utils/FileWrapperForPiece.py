
def get_file_wrapper_for_piece(piece, file):
    start_offset = None
    count_bytes = None
    
    if is_piece_in_file(piece, file):
        start_offset = piece.stream_offset - file.stream_offset
        count_bytes = piece.size
    elif is_piece_in_end_of_file(piece, file):
        start_offset = piece.stream_offset - file.stream_offset
        count_bytes = file.get_end_offset() - piece.stream_offset
    elif is_piece_in_begin_of_file(piece, file):
        start_offset = 0
        count_bytes = piece.get_end_offset() - file.stream_offset
    elif is_file_in_piece(piece, file):
        start_offset = 0
        count_bytes = file.size
    else:
        raise Exception
    
    fw = FileWrapperForPiece(start_offset, count_bytes, file)    
    return fw

# кусок полностью в файле
def is_piece_in_file(piece, file):
    fileBeginsBeforePieceBegins = file.stream_offset <= piece.stream_offset
    fileEndsAfterPieceEnds = file.get_end_offset() >= piece.get_end_offset()
    return fileBeginsBeforePieceBegins and fileEndsAfterPieceEnds

# кусок приходится на конец файла но в файле не заканчивается
def is_piece_in_end_of_file(piece, file):
    fileBeginsBeforePiece = file.stream_offset < piece.stream_offset
    fileEndsInsidePiece = file.get_end_offset() > piece.stream_offset and file.get_end_offset() < piece.get_end_offset()
    return fileBeginsBeforePiece and fileEndsInsidePiece

# кусок затрагивает начало файла, но в файле не начинается
def is_piece_in_begin_of_file(piece, file):
    fileBeginsInsidePiece = file.stream_offset > piece.stream_offset and file.stream_offset < piece.get_end_offset()
    fileEndsAfterPieceEnds = file.get_end_offset() > piece.get_end_offset()
    return fileBeginsInsidePiece and fileEndsAfterPieceEnds

# файл полностью в куске
def is_file_in_piece(piece, file):
    fileBeginsInsidePiece = file.stream_offset >= piece.stream_offset
    fileEndsInsidePiece = file.get_end_offset() <= piece.get_end_offset()
    return fileBeginsInsidePiece and fileEndsInsidePiece


# вспомогательный класс для хранения в piece списка файлов которые он(piece) "накрывает"
# содержит доп. информацию для облегчения считывания нужной части файла
class FileWrapperForPiece:
    def __init__(self, seek, read, reference_file):
        self.seek_offset = seek                     # смещение относительно начала файла с которого начинается piece
        self.read_offset = read                     # сколько нужно считать из файла данных
        self.reference_file = reference_file
        self.possible_file = None
 
    def get_all_possible_file(self):
        if self.reference_file.status == "MATCH_FOUND":
            return [self.reference_file.matched_file]
        else:
            return self.reference_file.possible_matches

    def get_data(self):
        import os
        data = b''
        fname = os.path.join(self.possible_file['path'], self.possible_file['name'])
        with open(fname, 'rb') as possible_file:
            possible_file.seek(self.seek_offset)
            data = possible_file.read(self.read_offset)
        
        return data

    def apply_match_file_as_positive(self):
        self.reference_file.matched_file = self.possible_file

    def update_status(self, status):
        if not self.reference_file.status == "MATCH_FOUND":
            self.reference_file.status = status
