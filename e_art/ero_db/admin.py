from django.contrib import admin
from ero_db.models import Country, BodyPart, Color, PhotoModel, Studio, AliasModel, Photographer, PubicHair, Genre, PhotoSet, Photo

# Register your models here.
class CountryAdmin(admin.ModelAdmin):
    list_display = ('name', 'code')
    search_fields = ('name', 'code')

admin.site.register(Country, CountryAdmin)
admin.site.register(BodyPart)
admin.site.register(Color)
admin.site.register(PhotoModel)
admin.site.register(Studio)
admin.site.register(AliasModel)
admin.site.register(Photographer)
admin.site.register(PubicHair)
admin.site.register(Genre)
admin.site.register(PhotoSet)
admin.site.register(Photo)
