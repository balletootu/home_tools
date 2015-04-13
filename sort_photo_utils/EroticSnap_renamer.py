import os

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

sets = []

def parce_csv(path):
    names = os.listdir(path = path)
    
    for item in names:
        item_path = os.path.join(path, item);
        if os.path.isfile(item_path) and ('.csv' in item):        
            f = open(item_path, 'r')
            
            for line in f:
                sets.append(line)
            f.close()
    
def get_date(folder_name):
    date = ''
    for i_set in sets:
        is_found = True
        set_name_component = i_set.lower().split(',')[4].split(' ')
        
        for j in set_name_component:
            if not (j in folder_name) :
                is_found = False
                break;
         
        if is_found:
            date = i_set.split(',')[1]
            date_component = date.split(' ')
            date = '{0}-{1}-{2}'.format(date_component[2], month[date_component[0]], date_component[1])
            return date;
            
    return date

def get_set_name(folder_name):
    set_name = folder_name
    folder_name_lo = folder_name.lower()
    for i_set in sets:
        is_found = True
        set_name_component = i_set.lower().split(',')[4].split(' ')
        
        for j in set_name_component:
            if not (j in folder_name_lo) :
                is_found = False
                break;
         
        if is_found:
            date = i_set.split(',')[1]
            date_component = date.split(' ')
            date = '{0}-{1}-{2}'.format(date_component[2], month[date_component[0]], date_component[1])
            
            set_inforamtion = i_set.split(',')
            set_name = '{0} - {1} - {2} (x{3})'.format(date, folder_name.split(' ')[0], set_inforamtion[4], set_inforamtion[5].split(' ')[0])
            return set_name;
            
    return set_name    
    
def make_renames(path, renames):
    names = os.listdir(path = path)
    
    for item in names:
        item_path = os.path.join(path, item);
        if os.path.isdir(item_path):
#            renames.append('{0} - {1}'.format(get_date(item), item.split('-')[1]))
            renames.append(get_set_name(item))
                        
if __name__ == "__main__":
    cur_dir = os.getcwd()

    parce_csv(os.path.join(cur_dir, '!csv'))
    
    renames = []
    make_renames(cur_dir, renames)
    
    f_out = open('rename.txt', 'w')
    for i in renames:
        f_out.write(i + '\n');

    f_out.close()