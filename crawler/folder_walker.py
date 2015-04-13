
"""
helper должен содержать следующие функции
get_start_folder() - возвращает стартовый путь, откуда будет начат обход дерева каталогов
get_content(item) - возвращает содержимое переданного каталога
is_folder(item) - проверяет является ли текущий элемент папкой
p.s. item должен быть единообразен для всех 3-х обязательных функций, он же и возвращается волкером, т.е. если это строки то доолжны быть строки во всех 3-х случаях, если это какая-то структура, то везде структура 
"""
class FolderWalker:
    def __init__(self, walk_helper):
        self.helper = walk_helper
        self.stack = [walk_helper.get_start_folder()]
        self.content = []
        self.index = 0

    def __getitem__(self, index):
        while 1:
            try:
                item = self.content[self.index]
                self.index = self.index + 1
            except IndexError:
                # pop next directory from stack
                self.directory = self.stack.pop()
                self.content = self.helper.get_content(self.directory)
                self.index = 0
            else:
                # got a filename
                if self.helper.is_folder(item):
                    self.stack.append(item)
                return item