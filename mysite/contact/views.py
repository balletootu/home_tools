from django.shortcuts import render_to_response
from contact.forms import ContactForm
from django.http.response import HttpResponseRedirect
from django.http import HttpResponse

def contact(request):
    if request.method == 'POST':
        form = ContactForm(request.POST)
        if form.is_valid():
            cd = form.cleaned_data
#            send_mail(cd['subject'], cd['message'], cd.get('e_mail', 'noreply@example.com'), ['siteowner@example.com'],)
            return HttpResponseRedirect('/contact/thanks/')
    else:
        form = ContactForm(initial = {'subject' : 'Fuck!!!'})
    
    return render_to_response('contact_form.html', {'form' : form})

def contact_thanks(reques):
    return HttpResponse('Thank you')