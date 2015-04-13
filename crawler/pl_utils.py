import http.cookiejar, urllib.request, urllib.parse
from asyncio.tasks import sleep

LOGIN = 'moloch_82'
PASS = 'hmEcQVVf3HYZnE6'

class PLUtils:
    def __init__(self):
        self.opener = None
        self.login_data = None
        self.authorization()
            
    def authorization(self):
        cookies = http.cookiejar.CookieJar()
        self.opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cookies))
        self.login_data = urllib.parse.urlencode({'login_username' : LOGIN, 
                                             'login_password' : PASS,
                                             'login' : ''}).encode(encoding='utf_8')
        self.opener.open('http://pornolab.net/forum/login.php', self.login_data)

    def download_torrents(self, tor_urls):
        h = self.opener.open('http://pornolab.net/forum/dl.php?t=1898890', self.login_data)        # без 2-го параметра вываливается страница о том что нельзя скачать торрент
        
        with open('1.torrent', 'wb') as f:
            f.write(h.read())
        
    def search(self):        
        s_data = {'prev_my' : '0',
                  'prev_new' : '0',
                  'prev_oop' : '0',
                  'f[]': '1726',
                  'o' : '1',
                  's' : '2',
                  'tm' : '-1',
                  'pn' :'',
                  'nm' :'',
                  'submit' : 'Поиск'
                  }
        s_data = urllib.parse.urlencode(s_data).encode(encoding='utf_8')
        h = self.opener.open('http://pornolab.net/forum/tracker.php', s_data)
        
        with open('1.html', 'wb') as f:
            f.write(h.read())
    
    
if __name__ == "__main__":
#    download_torrents(...)

    pl = PLUtils()
    tor_urls = []
    pl.download_torrents(tor_urls)
