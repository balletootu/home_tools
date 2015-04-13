import http_utils
import sys
import xml.etree.ElementTree as ET
from _elementtree import Element


AUTHORIZATION_PAGE = 'http://torrent-tv.ru/auth.php'
EMAIL = 'kovalas@gmail.com'
PASS = 's4ggSkkG28sz'

START_ID_MARK = 'this.loadPlayer("'
END_ID_MARK = '"'
TRANSLATION_TEMPLATE = 'http://torrent-tv.ru/torrent-online.php?translation=%d'

M3U_HEAD = '#EXTM3U\n'
M3U_CHANNEL_TEMPLATE = '#EXTINF:0,%s\n#EXTVLCOPT:network-caching=1000\n#EXTVLCOPT:deinterlace=1\n#EXTVLCOPT:deinterlace-mode=blend\n%s\n'

channel_id = {
    'Спорт 1 HD' : 420,
    'Спорт 1' : 383,
    'Россия 2' : 6886,
    'СТБ' : 5466 
    }

def get_content_id_from_page(page):
    try:
        start_idx = page.index(START_ID_MARK, 0, len(page))
        start_idx += len(START_ID_MARK) 
        end_idx = page.index(END_ID_MARK, start_idx, len(page))
    except:
            print ('Error: File doesn\'t contain markers or changed layouts ', sys.exc_info()[:2])
        
    return page[start_idx:end_idx]

def get_content_ids():    
    hd = http_utils.HTTPDownloader()
    aut_data = {
        'email' : EMAIL,
        'password' : PASS,
        'remember' : 'on',
        'enter' : 'Войти'
    }
    
    hd.authorization(AUTHORIZATION_PAGE, aut_data)

    with open('torrent_tv.m3u', encoding= 'utf-8', mode = 'w') as f:
        f.write(M3U_HEAD)        
        for i in channel_id.keys():
            cur_url = TRANSLATION_TEMPLATE % (channel_id[i])
            page = hd.download_html_page(cur_url)
            c_id = get_content_id_from_page(page)
            f.write(M3U_CHANNEL_TEMPLATE % (i, c_id))
            
def add_channel(element, name, content_id, group):
    track = ET.SubElement(element, 'track')
    ET.SubElement(track, 'location').text = content_id
    ET.SubElement(track, 'title').text = name
    ET.SubElement(track, 'album').text = group
    
    extension = ET.SubElement(track, 'extension', {'application' : 'http://www.videolan.org/vlc/playlist/0'})
    ET.SubElement(extension, 'vlc:option').text = 'network-caching=1000'
    ET.SubElement(extension, 'vlc:option').text = 'deinterlace=1'
    ET.SubElement(extension, 'vlc:option').text = 'deinterlace-mode=blend'
      
def make_xspf(name):
    playlist = ET.Element('playlist', {'xmlns':'http://xspf.org/ns/0/', 'xmlns:vlc':'http://www.videolan.org/vlc/playlist/ns/0/', 'version':'1'})
    tree = ET.ElementTree(element = playlist)
    
    ET.SubElement(playlist, 'title').text = 'torrent-tv.ru'
    track_list = ET.SubElement(playlist, 'trackList')
    add_channel(track_list, 'test', '12345', 'first')
    
    tree.write(name, encoding = 'utf-8', xml_declaration=True)
            
if __name__ == "__main__":
#    get_content_ids()
    make_xspf('torrent_tv.xspf',)
