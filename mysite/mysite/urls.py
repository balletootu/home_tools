from django.conf.urls import patterns, include, url
from django.contrib import admin

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'mysite.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^search/$', 'books.views.search'),
    url(r'^contact/$', 'contact.views.contact'),
    url(r'^contact/thanks/$', 'contact.views.contact_thanks'),
)

urlpatterns += patterns('mysite.views',
    url(r'^hello/$', 'hello'),
    url(r'^time/$', 'current_datetime'),
    url(r'^time/plus/(\d{1,2})/$', 'hours_ahead'),
    url(r'^meta/$', 'display_meta'),
)
