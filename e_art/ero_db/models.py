from django.db import models

# Create your models here.

"""
есть еще интересный сайт: http://www.indexxx.com/
"""

class Country(models.Model):
    name    = models.CharField(max_length = 30, unique = True)
    code    = models.CharField(max_length = 3, unique = True)         # http://en.wikipedia.org/wiki/ISO_3166-1_alpha-3

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

class BodyPart(models.Model):
    name    = models.CharField(max_length = 30, unique = True)        # названия брать отсюда http://www.thenude.eu/index.php?page=search&action=selectTattoos

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

class Color(models.Model):
    name    = models.CharField(max_length = 30, unique = True)        # для цвета волос, м.б. еще для чегото

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

class PhotoModel(models.Model):
    name                = models.CharField(max_length = 30)           # как вариант, брать такоеже название как и на thenude.eu
    first_name          = models.CharField(max_length = 30, blank = True)
    last_name           = models.CharField(max_length = 30, blank = True)
    ICGID               = models.CharField(max_length = 10, unique = True)           # International Cover Girl ID, похоже что есть у всех моделей и он уникальный
    born                = models.DateField(blank = True, null = True)
    birthplace          = models.ForeignKey(Country, blank = True)
    thenude_url         = models.URLField()                             # model's page on thenude.eu
    first_seen          = models.IntegerField(blank = True, null = True)                         # year
    last_seen           = models.IntegerField(blank = True, null = True)                         # year
    measurement_bust    = models.CharField(max_length = 10, blank = True)             # 34B/89    info:https://www.soma.com/web_assets/sizechart/size_chart.html?sizeOld=false
    measurement_waist   = models.IntegerField(blank = True, null = True)                         # см
    measurement_hips    = models.IntegerField(blank = True, null = True)                         # см
    height              = models.IntegerField(blank = True, null = True)                         # см
    weight              = models.IntegerField(blank = True, null = True)                         # кг
    breasts             = models.CharField(max_length = 10, blank = True)             # Medium, Small и т.д.
    is_silicone_bust    = models.BooleanField(default = False, blank = True)
    siliconed_date      = models.DateField(blank = True, null = True)
    hair_сolour         = models.ForeignKey(Color, blank = True)                      # в разных сетах бывают разный цвет волос
    piercings           = models.BooleanField(default = False, blank = True)
    where_piercing      = models.ManyToManyField(BodyPart, related_name = 'where_piercing', blank = True)
    tattoos             = models.BooleanField(default = False, blank = True)
    where_tattoo        = models.ManyToManyField(BodyPart, related_name = 'where_tattoo', blank = True)
    official_site       = models.URLField(blank = True)
    interview_page      = models.URLField(blank = True)
    social_network      = models.URLField(blank = True)
    agency              = models.URLField(blank = True)
    body_part_photos    = models.CharField(max_length = 200, blank = True)   
    """ несколько ссылок на картинки с изображением частей тела модели (грудь, половые губы, зад, ноги)???
        как вариант можно сделать более сложное описание с использованием координат на картинке. Пример: vagina:ссылка_на_фотку_в_какомто_виде: (100,150)x(300,400)
        1 - некоторый тег означающий часть тела (vagina, boobs, face и т.д.)
        2 - думаю, здесь лучше использовать не абсолютные пути(т.к. есть вероятность переезда фоток), а использовать структуры описывающие сет и фотки 
        3 - область, описывается координатами левого верхнего угла + ширина и высота (думаю, имеет смысл, использовать не абсолютнче координаты, а приведенные в диапозон [0:1], чтобы при подстановке этогоже изображения в другом разрешении, оставался валидным регион) """
    # + нужно придумать какуюто классификацию, еще не описанных частей тела. например форма груди, форма сосков, как выглядят половые губы (м.б. сделать шаблонные фотки или фотки "интересных" часте тела конкретной модели)

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name', 'ICGID']
    
class Studio(models.Model):
    name        = models.CharField(max_length = 20, unique = True)
    url         = models.URLField(unique = True)
    thenude_id  = models.IntegerField(blank = True, null = True)

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

class AliasModel(models.Model):
    studio_name     = models.CharField(max_length = 30)     # !!!! нужно переименовать
    photo_model     = models.ForeignKey(PhotoModel)
    studio          = models.ForeignKey(Studio)

    def __str__(self):
        return self.studio_name    

    class Meta:
        ordering = ['studio_name']
    
class Photographer(models.Model):
    name            = models.CharField(max_length = 30)         # псевдоним с thenude
    first_name      = models.CharField(max_length = 30, blank = True)         # реальное имя
    last_name       = models.CharField(max_length = 30, blank = True)         # реальная фамилия
    born            = models.DateField(blank = True, null = True)
    birthplace      = models.ForeignKey(Country, blank = True)
    official_site   = models.URLField(blank = True)

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']
    
class PubicHair(models.Model):
    name    = models.CharField(max_length = 30, unique = True)            # зафигачить названия лобковых причесок.... (bare, trimmed)

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

class Genre(models.Model):
    name    = models.CharField(max_length = 30, unique = True)            # брать отсюда http://pornolab.net/forum/viewtopic.php?t=985049

    def __str__(self):
        return self.name    
   
    class Meta:
        ordering = ['name']
 
class PhotoSet(models.Model):
    date                    = models.DateField()
    name                    = models.CharField(max_length = 100)
    photo_models            = models.ManyToManyField(PhotoModel, through='PhotoModel_PhotoSet', through_fields=('set', 'model'))
    photographer            = models.ForeignKey(Photographer)
    studio                  = models.ForeignKey(Studio)
    count_photos            = models.IntegerField(blank = True, null = True)
    is_video                = models.BooleanField(default = False)
    files_name_template     = models.CharField(max_length = 100, blank = True)
    gallery_url             = models.URLField(blank = True)

    def __str__(self):
        return self.name    

    class Meta:
        ordering = ['name']

    
class PhotoModel_PhotoSet(models.Model):            # связующая таблица связи мнокие-ко-многим таблиц модель<->сет
    model           = models.ForeignKey(PhotoModel)
    set             = models.ForeignKey(PhotoSet)
    hair_сolour     = models.ForeignKey(Color, blank = True)              # если не задано, то берем это значение из таблицы модели
    pubic_hair      = models.ForeignKey(PubicHair, blank = True)
    is_siliconed    = models.BooleanField(default = False)  # для моделей которые снимались как с силиконом так и без
    genre           = models.ManyToManyField(Genre, blank = True)         # жанр здесь, т.к. модель в сете может поразному быть вовлечена ... 
    
# для начала работаем только до уровня сетов, попозже попробую включить и описание фоток    
class Photo(models.Model):
    number          = models.IntegerField()                        # этот номер наложенный на шаблон из сета даст имя файла
    set             = models.ForeignKey(PhotoSet)
    photo_models    = models.ManyToManyField(PhotoModel, through = 'Photo_PhotoModel', through_fields = ('photo', 'photo_model')) 

    def __str__(self):
        return self.number   

    class Meta:
        ordering = ['number']
    
class Photo_PhotoModel(models.Model):               # связующая таблица связи мнокие-ко-многим таблиц моель<->фото
    photo_model     = models.ForeignKey(PhotoModel)
    photo           = models.ForeignKey(Photo)
    # подумать как реализовать и добавить полей для описания модели на определенной фотке (степень одетости/раздетости, что показала, )
    