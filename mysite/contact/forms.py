from django import forms

class ContactForm(forms.Form):
    subject = forms.CharField(max_length = 100)
    e_mail = forms.EmailField(required = False, label = 'E-mail')
    message = forms.CharField(widget = forms.Textarea)