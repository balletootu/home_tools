import os
from utils import get_range
import torrent_utils

def matching_files_by_size(torrent, exist_files):
    tor_name = torrent_utils.get_torrent_name(torrent)
    files = []
    
    if torrent_utils.is_single_file_torrent(torrent):
        size = torrent_utils.get_payload_size(torrent)
        files.append( TFile('', tor_name, size, 0) )
    else:        
        tor_files = torrent['info']['files']
        stream_offset = 0
        for file in tor_files:
            fsize = file['length']
            matched_files = get_range(exist_files, fsize, lambda x: x['len'])
            fpath = os.path.join(tor_name, *file['path'][:-1])
            fname = file['path'][-1]
            files.append(TFile(fpath, fname, fsize, stream_offset, matched_files))
            stream_offset += fsize

    return files

class TFile:
    def __init__(self, path, file_name, size, stream_offset, m_files):
        self.path = path
        self.name = file_name
        self.size =size
        self.stream_offset = stream_offset
        self.matched_file = None
        self.possible_matches = m_files
        self.status = 'NOT_CHECKED'
        
    def get_end_offset(self):
        return self.stream_offset + self.size
    
    def get_path_from_torrent(self):
        return os.path.join('', self.path)

    def get_torrent_name(self):
        return os.path.join(self.get_path_from_torrent(), self.name)
        
    def get_matched_name(self):
        return os.path.join(self.matched_file['path'], self.matched_file['name'])
    
    def is_matched(self):
        return (self.status == 'MATCH_FOUND')
        
    def is_intersect_with(self, piece):
        fEndOffset = self.get_end_offset()
        pEndOffset = piece.get_end_offset()
        
        pieceIsWholelyContainedInFile = ( self.stream_offset <= piece.stream_offset and fEndOffset >= pEndOffset )
        
        fileBeginsBeforePieceBegins = self.stream_offset <= piece.stream_offset
        fileEndsAfterPieceBegins = fEndOffset > piece.stream_offset
        
        fileBeginsInsidePiece = self.stream_offset < pEndOffset
        fileEndsAfterPieceEnds = fEndOffset > pEndOffset
        
        fileIsPartiallyContainedInPiece = (fileBeginsBeforePieceBegins and fileEndsAfterPieceBegins) or (fileBeginsInsidePiece and fileEndsAfterPieceEnds)
        
        fileBeginsAfterPieceBegins = self.stream_offset >= piece.stream_offset
        fileEndsBeforePieceEnds = fEndOffset <= pEndOffset
        
        fileIsWholelyContainedInPiece = fileBeginsAfterPieceBegins and fileEndsBeforePieceEnds
        return pieceIsWholelyContainedInFile or fileIsWholelyContainedInPiece or fileIsPartiallyContainedInPiece
                