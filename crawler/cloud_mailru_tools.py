"""
описание части JSON-а находящегося в  "cloudBuilder({ ... });" а именно поля "folders" (остальное для браузинга по папкам не интересно)
folders (map)
    tree (список)   - спицефично описывает иерархию, в основном описывает только папки, но если мы пришли в самый конец иерархии, т.е. в папке будут только файлы, то опишет и файлы, а если в папке есть и файлы и папки, то описываются только папки
        уровень (map)   1 элемент списка описывает 1 уровень начиная с коревого пример: "http://..public_url../home/2010/01/" - 1-й элемент описывает "/", 2-й - "/home/", 3-й - "/home/2010/", 4-й - "/home/2010/01/" 
            count (map)
                folders     - количество папок
                files       - количество файлов
            list (список)   - ?? в нем присутствуют только папки, файлы в этом списке, почемуто не описываются
                ...
                items (список) то что содержится в этой папке (если текущая папка "не раскрыта", т.е. не участвует в пути для текущей открытой папки, то она содержит пустой список)
                ...
    folder(map)     - браузить лучше используя это описание, т.к. здесь полноценно описываются все дочерние элементы текущей папки
        count (map)
            folders     - количество папок
            files       - количество файлов
        name            - имя текущего элемента (скорее всего папки, т.к. файл врятли будет на этом уровне)
        kind, type      - оба поля здесь содержат значение "folder" (зачем 2 одинаковых поля?, м.б. в будующем будут какие-то изменения)
        weblink,id      - ссылка не тек. элемент (для абсолютного пути в начало нужно прибавить "https://cloud.mail.ru/public/"), оба поля содержат одинаковые значения, но, судя по назвнию, для браузинга лучше использовать weblink.
        list (список)   - элементы содержащиеся в текущей папке
            name
            weblink, id - ссылка на элемент
            kind, type  - file/folder
            parent      - линк на родителя
            ------- для file ---------
            hash        - хэш код
            size
            mtime
            ------- для folder ---------
            count
                folders
                files
            items (список)
            ...
"""

import http_utils
from sets_list import Sets_list
import json
import sys
import folder_walker

START_JSON_MARK = 'cloudBuilder({'
END_JSON_MARK = '});'
CMRU_URL_PREFIX = 'https://cloud.mail.ru/public/'

class CloudMailruTools:
    def __init__(self):
        self.downloader = http_utils.HTTPDownloader()
        
    def get_folder_content(self, url):
        """
        для заданного url возвращает список объектов находящихся в данной папке
        каждый элемент списка это map, который содержит:
        'name' - имя элемента
        'type' - тип элемента (file/folder)
        'url' - ссылка на этот элемент
        'size' - для файла возвращается размер
        """
        content = []        

        try:        
            page = self.downloader.download_html_page(url)
        except Exception:
            print ('Error in download_html_page()', sys.exc_info()[:2], ' for url:', url)
            return content

        try:
            start_idx = page.index(START_JSON_MARK, 0, len(page)) 
            start_idx += len(START_JSON_MARK) - 1
            end_idx = page.index(END_JSON_MARK, start_idx, len(page)) + 1    
            
            cloud_folder = page[start_idx:end_idx]
            cloud_folder = json.loads(cloud_folder)
            cloud_folder = cloud_folder['folders']['folder']['list']
            
            for item in cloud_folder:
                desc = {'type' : item['kind'], 'name' : item['name'], 'url' : CMRU_URL_PREFIX + item['weblink']}
                if  item['kind'] == 'file':
                    desc['size'] = int(item['size'])
                content.append(desc)
        except Exception:
            print ('Error: File contain wrong tree-description section or changed layouts ', sys.exc_info()[:2])

        return content
    

"""
хэлпер для волкера. использовать через вызов get_folder_walker()
"""    
class CMRUWalkHelper:
    def __init__(self, start_folder):
        fold_component = start_folder.split('/')
        name = fold_component[-1]
        if len(name) == 0:
            name = fold_component[-2]
            
        self.start_dir = {'name': name, 'type':'folder', 'url' : start_folder}
        
        self.tools = CloudMailruTools()

    def get_start_folder(self):
        return self.start_dir
    
    def get_content(self, item):
        return self.tools.get_folder_content(item['url'])
    
    def is_folder(self, item):
        return item['type'] == 'folder'
    
def get_folder_walker(url):
    return folder_walker.FolderWalker(CMRUWalkHelper(url))

"""
нужно реализовать более вразумительную функцию для проверки является ли имя именем сета или нет
"""
def is_set_name(name):
    if len(name.split('-')) > 4  : return True
    
    return False

def parse_public_folder(url, sets):
    f_walker = get_folder_walker(url)
     
    for item in f_walker:
        if item['type'] == 'folder' and is_set_name(item['name']):
            sets.add_set(item['name'])
            print(item['name'])

if __name__ == "__main__":
    sets = Sets_list()    
    parse_public_folder('https://cloud.mail.ru/public/3fdb7cb3d720/watch4beauty.com/2013/02/', sets)