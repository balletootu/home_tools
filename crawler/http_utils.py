import http.cookiejar, urllib.request, urllib.parse

class HTTPDownloader:
    def __init__(self):
        self.opener = urllib.request.build_opener()
        self.login_data = None
        self.set_opener_opt()
        
    def set_opener_opt(self):
        user_agent = r'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36'
        accept = r'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8'
        accept_encoding = r'gzip,deflate'
    
        self.opener.addheaders = [('Accept', accept),
                             ('User-Agent', user_agent),                    # некоторые сайты хотят это поле
                             ('Accept-Encoding', accept_encoding)]          # архивированные страницы значительно быстрее качаются

    def authorization(self, url, aut_data):
        cookies = http.cookiejar.CookieJar()
        self.opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cookies))
        self.login_data = urllib.parse.urlencode(aut_data).encode(encoding='utf_8')
        self.opener.open(url, self.login_data)
        self.set_opener_opt()

    # качает данные по url отдает их в виде bytes
    # is_torrent чтобы можно было скачать торрент например с rutracker
    def download_by_url(self, url, is_torrent = False):
        data = b''
        
        url = urllib.parse.quote(url, safe = '/:?=&')               # небольшая конвертация, потому, что некоторые url с пробелами дают сбой
        print(url)
        usock = None
        if is_torrent:
            usock = self.opener.open(url, self.login_data)       # пока, не знаю почему но без 2-го параметра вылетает сообщение (на рутрекере и т.д.), что торрент можно качать только с сайта
        else:
            usock = self.opener.open(url)
        
        url = usock.geturl()
        data = decode(usock)
        usock.close() 
        print(len(data), '=>', url)
                
        return data
    
    # для скачивания html-страниц. результат - список строк (отличие от предидушей ф-ии, делает декодинг в str и разбиение на строки)
    def download_html_page(self, url):
        """
        !!! нужно переделать все парсеры сайтов, т.к. раньше функция возвращала страницу разделенную на строки, а теперь все одной строкой!!!!!
        """
        data = self.download_by_url(url)
        data = data.decode('UTF-8', 'ignore')
        return data
    
def decode (page):
    import io
    import zlib, gzip
    
    encoding = page.info().get("Content-Encoding")    
    if encoding in ('gzip', 'x-gzip', 'deflate'):
        content = page.read()
        if encoding == 'deflate':
            data = io.BytesIO(zlib.decompress(content))
        else:
            data = gzip.GzipFile('', 'rb', 9, io.BytesIO(content))
        page = data.read()

    return page
    