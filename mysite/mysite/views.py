from django.template import Context
from django.shortcuts import render_to_response
from django.http import HttpResponse, Http404
import datetime

def hello(request):
    return HttpResponse("Hello word")

def current_datetime(request):
    current_date = datetime.datetime.now()
    return render_to_response('current_datetime.html', locals())

def hours_ahead(request, hour_offset):
    try:
        hour_offset = int(hour_offset)
    except ValueError:
        raise Http404()

    next_time = datetime.datetime.now() + datetime.timedelta(hours = hour_offset)
    return render_to_response('hours_ahead.html', locals())

def display_meta(request):
    meta_list = request.META.items()
    return render_to_response('meta.html', Context({'meta_list' : meta_list})) 