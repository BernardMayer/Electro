# script de.py
#(C) Fabrice Sincère 
# http://fsincere.free.fr/isn/python/cours_python_tkinter.php
# http://effbot.org/tkinterbook/
# https://www.tutorialspoint.com/python/tk_spinbox.htm

from tkinter import *
import random
import os


def NouveauLance():
    nb = random.randint(1,6)
    Texte.set('Résultat -> ' + str(nb))

# Def de la commande pour recuperer la date et l'heure
dateGetCmd    = 'date /T'
timeGetCmd    = 'time /T'
dateGetResult = os.popen(dateGetCmd).read().rstrip() #python2 [:-1]
timeGetResult = os.popen(timeGetCmd).read()[:-1]
#print("[" + dateGetResult + "] [" + timeGetResult + "]")


    
# Création de la fenêtre principale (main window)
w = Tk()
## Taille des caracteres 
w.option_add( "*font", "lucida 14 bold italic" )


# default_font = tkFont.nametofont("TkDefaultFont")
# default_font.configure(size=48)

w.title('Get RTC date and time')
w.geometry('800x300+400+400')
w['bg'] = 'bisque'

FrameTitle = Frame(w, borderwidth=2, relief=GROOVE, bg="white")
FrameTitle.pack(side=TOP, padx=5, pady=5)
LabelTitle = Label(FrameTitle, text="Mise a l'heure de la framboise", fg="navy")
LabelTitle.pack(padx=5, pady=5)#side=LEFT, 

FrameDate = Frame(w, borderwidth=2, relief=GROOVE)
FrameDate.pack(padx=10,pady=10)
vYear = StringVar(FrameDate)
vYear.set(2021)

##  Annee
wYear = Spinbox(master=FrameDate, width_=4, from_=2017, to_=2024, textvariable=vYear)
wYear.pack(side = LEFT, padx = 5, pady = 5)
##  Mois
wMonth = Spinbox(master=FrameDate, width_=2, from_=1, to_=12)
wMonth.pack(side = LEFT, padx = 5, pady = 5)
##  Jour
wDay = Spinbox(master=FrameDate, width_=2, from_=1, to_=31)
wDay.pack(side = LEFT, padx = 5, pady = 5)

FrameTime = Frame(w, borderwidth=2, relief=GROOVE)
FrameTime.pack(padx=10,pady=10)#side=LEFT,
##  Heure
wHour = Spinbox(master=FrameTime, width_=2, from_=0, to_=23)
wHour.pack(side = LEFT, padx = 5, pady = 5)
##  Mois
wMinute = Spinbox(master=FrameTime, width_=2, from_=0, to_=59)
wMinute.pack(side = LEFT, padx = 5, pady = 5)
##  Jour
wSecond = Spinbox(master=FrameTime, width_=2, from_=0, to_=59)
wSecond.pack(side = LEFT, padx = 5, pady = 5)

FrameAction = Frame(w, borderwidth=2, relief=GROOVE)
FrameAction.pack(padx=5, pady=5)#side=BOTTOM, 

# Création d'un widget Button (bouton Lancer)
# BoutonLancer = Button(FrameAction, text ='Lancer', command = NouveauLance)
# Positionnement du widget avec la méthode pack()
# BoutonLancer.pack(side = LEFT, padx = 5, pady = 5)

# Création d'un widget Button (bouton Quitter)
BoutonQuitter = Button(FrameAction, text ='Quitter', command = w.destroy)
BoutonQuitter.pack(side = LEFT, padx = 5, pady = 5)

Texte = StringVar()
NouveauLance()

# Création d'un widget Label (texte 'Résultat -> x')
LabelResultat = Label(FrameAction, textvariable = Texte, fg ='red', bg ='white')
LabelResultat.pack(side = LEFT, padx = 5, pady = 5)

w.mainloop()