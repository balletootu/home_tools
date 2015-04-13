import thenude_tools
from cloud_mailru_tools import parse_public_folder
from sets_list import Sets_list
from thenude_tools import TheNude_utils

def check_downloaded_sets(studio_fname, cloud_studio_lnk):
    sets = []
    
    with open(studio_fname, 'rb') as f:
        sets = f.read().decode('utf-8').split('\r\n')
        
#     print(sets)
#     print(len(sets))
        
    tmp_comp = cloud_studio_lnk.split(r'/')
    if len(tmp_comp[-1]) == 0:
        folder_id = '%s/%s' % (tmp_comp[-3], tmp_comp[-2])
    else:
        folder_id = '%s/%s' % (tmp_comp[-2], tmp_comp[-1])
     
    cloud_sets = Sets_list()
     
    parse_public_folder(cloud_studio_lnk, folder_id, cloud_sets)
 
    with open('not_exist.txt', 'w') as f:
        for i in sets:
            try:
                set_comp = i.split('|')
                set_name = '%s - %s - %s' % (set_comp[0], set_comp[1], set_comp[2])
                if not cloud_sets.is_exist(set_name):
                    f.write(i + '\n')
            except:
                print('ERROR check is_exist for set: ', i)

if __name__ == "__main__":
#     models = ['Jenya%20D_2351', 
#               'Liza%20B_2326',
#               'Anna%20Aj_9936',
#               'Marketa%20Belonoha_3147',
#               'Candice_23595',
#               'Carisha_21154',
#               'Dominica_13091',
#               'Iveta%20B_1872']
#        
#     for model in models:
#         get_model_sets_from_thenude(model)

    check_downloaded_sets(r'.\\studio\\EvasGarden.txt', 'https://cloud.mail.ru/public/88c08c0ed836/EvasGarden/')

#    thenude_tools.test_parse_page('http://www.thenude.eu/index.php?page=covers&action=showCovers&affiliate_code_id=566&year=2010&month=9')

# загрузка студийных сетов с thenude
#     sets = []
#     tnu = TheNude_utils()
#     tnu.get_studio_sets(6, sets)