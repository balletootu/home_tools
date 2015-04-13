
def is_single_file_torrent(bdata):
    return 'length' in bdata['info'].keys()

def get_torrent_name(bdata):
    return bdata['info']['name']

def get_piece_size(bdata):
    return bdata['info']['piece length']

def get_payload_size(bdata):
    if is_single_file_torrent(bdata):
        return bdata['info']['length']
    else:
        payloadSize = 0
        for f in bdata['info']['files']:
            payloadSize += f['length']
        return payloadSize


def get_final_piece_size(bdata):
    return get_payload_size(bdata) % get_piece_size(bdata)


def get_hashes_from_metadata(bdata):
    bd_hashes = bdata['info']['pieces']    
    SHA1_HASH_LENGTH = 20
    return [bd_hashes[start:start+SHA1_HASH_LENGTH] for start in range(0, len(bd_hashes), SHA1_HASH_LENGTH)]


def print_list(bdata):
    for i in bdata:
        if type(list()) == type(i): print_list(i)
        elif type(dict()) == type(i): print_dict(i)
        else: print(i)

def print_dict(bdata):
    for i in bdata.keys():        
        if type(list()) == type(bdata[i]):
            print('Section ', i, ' --->') 
            print_list(bdata[i])
        elif type(dict()) == type(bdata[i]): 
            print('Section ', i, ' --->') 
            print_dict(bdata[i])
        else: print(i, '->', bdata[i])