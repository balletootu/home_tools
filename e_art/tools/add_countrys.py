from ero_db.models import Country

# для добавления стран в таблицу, находясь в папке проекта нужно в консоли запустить: manage.py shell
# и там набрать import tools.add_countrys.py (при этом файлик со странами желательно положить в корневую папку проекта)
with open('countrys.txt', encoding = 'utf-8', mode = 'r') as f:
    for line in f:
        line_component = line.strip().split('\t')
        print (line_component)
        p = Country(name = line_component[1], code = line_component[0])
        p.save()
