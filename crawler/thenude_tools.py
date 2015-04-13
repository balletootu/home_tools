import http_utils

url_template_thenude_per_page = 'http://www.thenude.eu/%s.htm?page_nr=%d&filter_per_page=30&filter_affiliate_code_id=&filter_sort=newest#Covers'

URL_TEMPLATE_STUDIO = 'http://www.thenude.eu/index.php?page=covers&action=showCovers&affiliate_code_id=%d'
AUTHORIZATION_PAGE = 'http://www.thenude.eu/index.php?page=login&action=login'
LOGIN = 'onieranel'
PASS = '81BL20e13xb4m81'

START_NAV_SECTION = 'class="nav"'
END_NAV_SECTION = '</div>'

def parse_date(date):    
    month = {'January': '01', 
             'February': '02',
             'March': '03',
             'April': '04',
             'May': '05',
             'June': '06',
             'July': '07',
             'August': '08',
             'September': '09',
             'October': '10',
             'November': '11',
             'December': '12'}    

    date_component = date.split(' ')
    
    if len(date_component) > 2: 
        return '%s-%s-%02d' % ( date_component[2], month[date_component[1]], int(date_component[0]))
    else:
        return ''
    
def next_find(html_strings, start_index, attribute):
    while True:
        if attribute in html_strings[start_index]:
            return start_index, html_strings[start_index]
            break
        start_index += 1

class TheNude_utils:
    def __init__(self):
        self.worker = http_utils.HTTPDownloader()
        aut_data = {
            'redirect_to' : 'http%253A%252F%252Fwww.thenude.eu%252Findex.php%253Fpage%253Dindex',
            'initiated_from' : 'http%253A%252F%252Fwww.thenude.eu%252Findex.php%253Fpage%253Dindex',
            'username' : LOGIN,
            'password' : PASS,
            'remember' :'on',
            'login' : 'Login'
        }
        
        self.worker.authorization(AUTHORIZATION_PAGE, aut_data)

    def get_model_sets(self, model_lnk):
        sets = []
        
        idx = 1
        while True :
            url = url_template_thenude_per_page % (model_lnk, idx)
            page = self.worker.download_page_by_url(url)
            if self.parse_thenude_page(page, sets) == 0: break
            idx += 1
            
        with open('%s.txt' % (model_lnk), 'w') as f:
            for i in sets:
                f.write(i + '\n');
                
    def parse_href_section(self, page, start_idx, end_idx):
        urls = []
        try:
            while True:
                start_idx = page.index('href=', start_idx, end_idx)
                end_href_idx = page.index('>', start_idx, end_idx)
                url = page[start_idx:end_href_idx].split('"')[1]
                urls.append(url)
                start_idx = end_href_idx
        except:
            start_idx = end_idx         # do everything
            
        return urls  

    def get_studio_sets(self, studio_id, sets):
        studio_lnk = URL_TEMPLATE_STUDIO % (studio_id)
        page = self.worker.download_html_page(studio_lnk)
            
        start_idx = page.index(START_NAV_SECTION, 0, len(page))
        end_idx = page.index(END_NAV_SECTION, start_idx, len(page))

        years_lnk = self.parse_href_section(page, start_idx, end_idx)
                        
        for item in years_lnk:
            self.parse_year_page(item, sets)
    
        with open('%d.txt' % (studio_id), 'wb') as f:
            for i in sets:
                f.write((i + '\r\n').encode('utf-8') );

    def parse_year_page(self, year_url, sets):
        page = self.worker.download_html_page(year_url)    
    
        start_idx = page.index(START_NAV_SECTION, 0, len(page))
        start_idx = page.index(START_NAV_SECTION, start_idx + 1, len(page))     # need second nav section with month urls
        end_idx = page.index(END_NAV_SECTION, start_idx, len(page))

    
        index, str_tmp = next_find(page, 0, 'class="nav"')
        index, str_tmp = next_find(page, index, '<span>month:')
        index += 1
        
        while True:
            if 'href=' in page[index]:
                if not ('disable' in page[index]):
                    month_url = page[index].split('href="')[1].split('"')[0]
                    self.parse_thenude_page(month_url, sets)
            else:
                break
            index += 1

    def parse_thenude_page(self, page_url, sets):
        page = self.worker.download_html_page(page_url)
        sets_count = 0
        
        for idx in range(len(page)):
            cur_line = page[idx]
            if ('class="item"' in cur_line) and ('style="position:' in cur_line):
                set_info = self.parse_set_information(page, idx)
                print(set_info)
                if set_info == 'video':
                    continue
                sets_count += 1
                sets.append(set_info)
                
        return sets_count

    def parse_set_information(self, html_strings, index):    
        index, date = next_find(html_strings, index, 'datePublished')
            
        index += 1
        name = html_strings[index]
        
        index, set_name = next_find(html_strings, index, '<span>')    
        index, photographer = next_find(html_strings, index, 'author')      
        index, count_image = next_find(html_strings, index, '<span>')          
        index, publisher = next_find(html_strings, index, 'publisher')
        
        count_image = count_image.split('>')[1].split('<')[0].split(' ')[0]
        if count_image == 'video':
            return 'video'
         
        date = date.split('>')[1].split('<')[0]
        date = parse_date(date)
             
        name = name.strip()
         
        set_name = set_name.split('>')[1].split('<')[0].strip()
         
        if '<a' in photographer:
            photographer = photographer.split('<a')[1].split('>')[1].split('<')[0]
        else:
            photographer = ''
             
        publisher_id = publisher.split('code_id=')[1].split('"')[0]
        publisher = publisher.split('<a')[1].split('>')[1].split('<')[0]
         
        return '%s|%s|%s|%s|%s|%s|%s' % (date, name, set_name, photographer, count_image, publisher, publisher_id)
    
    def new_parse(self, url):
        import lxml.html as html
        page = self.worker.download_html_page(url)
        page = html.document_fromstring(page)
        nav = page.find_class('nav')
#        urls = nav[0].iterlinks()
        for i in nav[0].iterlinks():        # ссылки на года
            print(i[2])

        for i in nav[1].iterlinks():        # ссылки на месяцы, но сюда попадают вот такие строки "javascript:void(0);"
            print(i[2])

        items = page.find_class('item')
        print('count sets', len(items))

        print(html.tostring(items[0]))
        for i in items[0]:                  # описание 1-го сета
#            print(i , '=>', i.text_content(), '=>', i.text)
            print(html.tostring(i))
            
#         item = items[0]
#         print(item.xpath('//span[@itemprop="datePublished"').text)

def test_parse_page(url):    
    tnu = TheNude_utils()
    sets = []
    tnu.parse_thenude_page(url, sets)

    with open('tmp.txt', 'w') as f:
        for i in sets:
            f.write(i + '\n')
            
studios_id = {
              'joymii' : 566 , 
              'sexart' : 670 ,
              'x-art' : 329 ,
              'femjoy' : 3 ,
              'hegre-art' : 11 ,
              'errotica-archives' : 2 ,
              'watch4beauty' : 13 ,
              'evasgarden' : 33 ,
              'metart' : 6 ,
              'alsscan' : 464 ,
              'eroticbeauty' : 656 ,
              'zemani' : 210 ,
              'eternaldesire' : 741 ,
              'mc-nudes' : 1 ,
              'rylskyart' : 718
              }

if __name__ == "__main__":
    tnu = TheNude_utils()
    tnu.new_parse('http://www.thenude.eu/index.php?page=covers&affiliate_code_id=6')