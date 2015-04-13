# моя реализация bencode-ра
# реализация 2-х проходная. Сначала вытягиваем все в bytes, 
# затем находим поле с кодировкой и с этой кодировкой декодим все строковые поля из bytes в str

def decode_list(bdata, pos):
    blist, pos = [], pos+1
    while bdata[pos:pos+1] != b'e':
        val, pos = decode_func[bdata[pos:pos+1]](bdata, pos)
        blist.append(val)
        
    return blist, pos + 1
    
def decode_dict(bdata, pos):
    bdict, pos = {}, pos+1
    while bdata[pos:pos+1] != b'e':
        key, pos = decode_string(bdata, pos)
        bdict[key], pos = decode_func[bdata[pos:pos+1]](bdata, pos)
        
    return bdict, pos + 1
    
def decode_int(bdata, pos):
    pos += 1
    end_pos = bdata.index(ord('e'), pos)
    val = int(bdata[pos:end_pos])

    return val, end_pos + 1 

def decode_string(bdata, pos):
    delim_pos = bdata.index(ord(':'), pos)
    length = bdata[pos:delim_pos]
    length = int(length) 
    
    delim_pos += 1
    bstring = bdata[delim_pos:delim_pos + length]

    if len(bstring) != length:
        raise ValueError("Incorrect bencoded string length")

    return bstring, delim_pos + length
    
decode_func = {
               b'l' : decode_list,
               b'd' : decode_dict,
               b'i' : decode_int,
               b'0' : decode_string,
               b'1' : decode_string,
               b'2' : decode_string,
               b'3' : decode_string,
               b'4' : decode_string,
               b'5' : decode_string,
               b'6' : decode_string,
               b'7' : decode_string,
               b'8' : decode_string,
               b'9' : decode_string}

def decode_to_bytes(bdata):
    val, pos = decode_func[bdata[0:1]](bdata, 0)
    
    if pos != len(bdata):
        raise KeyError("invalid bencoded value (data after valid prefix)")
    
    return val

def decode_blist(bdata, encoding):
    new_list = []
    for item in bdata:
        if type(list()) == type(item): new_list.append(decode_blist(item, encoding))
        elif type(dict()) == type(item): new_list.append(decode_bdict(item, encoding))
        else: new_list.append(item.decode(encoding))
        
    return new_list

def decode_bdict(bdata, encoding):
    new_dict = {}
    for item in bdata.keys():
        decoded_key = item.decode(encoding)
        if type(list()) == type(bdata[item]):
            new_dict[decoded_key] = decode_blist(bdata[item], encoding)
        elif type(dict()) == type(bdata[item]): 
            new_dict[decoded_key] = decode_bdict(bdata[item], encoding)
        elif type(int()) == type(bdata[item]) or (decoded_key == 'pieces'):
            new_dict[decoded_key] = bdata[item] 
        else: 
            new_dict[decoded_key] = bdata[item].decode(encoding) 
            
    return new_dict

def decode_bdata(bdata, encoding):
    return decode_bdict(bdata, encoding)

def decode(bdata):
    torrent = decode_to_bytes(bdata)
    
    tor_encoding = torrent[b'encoding'].decode('utf-8', 'ignore')
    return decode_bdata(torrent, tor_encoding)
