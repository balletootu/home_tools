from django.shortcuts import render, render_to_response
from django.http.response import HttpResponse
from books.models import Book

# Create your views here.

def search(request):
    errors = []
    if 'q' in request.GET: 
        q = request.GET['q']
        if not q:
            errors.append('Введите поисковый запрос.')
        elif len(q) > 20:
            errors.append('Введите не более 20 символов.')
        else:
            books = Book.objects.filter(title__icontains = q)
            return render_to_response('search_result.html', {'books' : books, 'query' : q})
    
    return render_to_response('search_form.html', {'errors' : errors})
