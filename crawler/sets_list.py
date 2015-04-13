
# сеты хранятся в структуре - словарь годов, внутри словарь месяцев, внутри словарь дней и последний, список сетов

class Sets_list:
    def __init__(self):
        self.sets_ = {}
        
    def get_date(self, set_name):
        set_name_comp = set_name.split('-')
        
        return int(set_name_comp[0].strip()), int(set_name_comp[1].strip()), int(set_name_comp[2].strip())
    
    def updte_struct(self, year, month, day):    
        if not (year in self.sets_) :
            self.sets_[year] = {}
            self.sets_[year][month] = {}
            self.sets_[year][month][day] = []
        elif not (month in self.sets_[year]) :
            self.sets_[year][month] = {} 
            self.sets_[year][month][day] = []
        elif not (day in self.sets_[year][month]) :
            self.sets_[year][month][day] = []
            
    def is_struct_exist(self, year, month, day):
        if not (year in self.sets_) or not (month in self.sets_[year]) or not (day in self.sets_[year][month]):
            return False    
        return True

    def add_set(self, set_name):
        try:
            if len(set_name.split('-')) < 5: return
            if self.is_exist(set_name): return
            year, month, day = self.get_date(set_name)
            
            self.updte_struct(year, month, day)
            self.sets_[year][month][day].append(set_name)
        except:
            print ('ERROR into add_set() for set_name:', set_name)
        
    def is_exist(self, set_name):
        year, month, day = self.get_date(set_name)
        
        if not self.is_struct_exist(year, month, day): return False
        
        if day in self.sets_[year][month] :
            for item in self.sets_[year][month][day]:
                if self.is_equal_set_names(set_name, item) :
                    return True
                
        return False
    
    # пример правильного имени сета: 2008-07-23 - Calandria - Dangerous Curves (x64) 2600x4000
    # - разделители '-'
    # - дата 'год-месяц-день'
    # - состоит из дата - имя модели(ей через &) - название сета (студийное) (xкол-во фото)
    def is_equal_set_names(self, set1, set2):
        set1 = set1.lower()
        set2 = set2.lower()

        set1_comp = set1.split('-')
        if len(set1_comp) < 5: print('Bed name set format:', set1)

        date = '%s-%s-%s' % (set1_comp[0].strip(), set1_comp[1].strip(), set1_comp[2].strip())
        models = set1_comp[3].split('&')
        set_name = set1_comp[4].split('(')[0].strip().split(' ')
        
        if not (date in set2): return False
        
        for i in models:
            if not (i.strip() in set2) : return False
            
        # временное решение для сравнения имени сета. Думаю прийдеться переделывать, т.к. может быть проблема с артиклями и недопустимыми для имен файлоов символами 
        for i in set_name:
            if not (i.strip() in set2) : return False
        
        return True
                
if __name__ == "__main__":
    
    sets = Sets_list()
    sets.add_set('2008-07-23 - Calandria - Dangerous Curves (x64) 2600x4000')
    sets.add_set('2008-08-08 - Loretta - Simplicity (x57) 4000 pixels')
    sets.add_set('2008-08-08 - Ashley - Totem (x28) 4000 pixels')
    sets.add_set('2008-08-08 - Ashley - Totem (x28) 4000 pixels')
    sets.add_set('2008-04-28 - Manuela - Fire (x36) 2667x4000')    
    
    print(sets.is_exist('2008-08-08-loretta-simplicity'))