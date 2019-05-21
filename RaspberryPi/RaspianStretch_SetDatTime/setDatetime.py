# script de.py
#(C) Fabrice Sincère 
# http://fsincere.free.fr/isn/python/cours_python_tkinter.php
# http://effbot.org/tkinterbook/
# https://www.tutorialspoint.com/python/tk_spinbox.htm

from tkinter import *
import random
import os

def SetDateTime():
    annee = wYear.get()
    # TODO : Mettre les zeros non significatifs !
    mois = wMonth.get()
    jour = wDay.get()
    heure = wHour.get()
    minutes = wMinute.get()
    secondes = wSecond.get()
    Texte.set(annee + "-" + mois + "-" + jour + " " + heure + ":" + minutes + ":" + secondes)

def NouveauLance():
    nb = random.randint(1,6)
    Texte.set('Résultat -> ' + str(nb))

# print("[" + os.name + "]")
# [nt] 'posix', 'nt', 'java'.
    
# Def de la commande pour recuperer la date et l'heure
if (os.name == "nt") :
    dateGetCmd    = 'date /T' # Windows
    timeGetCmd    = 'time /T' # Windows
    dateGetResult = os.popen(dateGetCmd).read().rstrip() #python2 [:-1]
    timeGetResult = os.popen(timeGetCmd).read()[:-1]
    # print("[" + dateGetResult + "] [" + timeGetResult + "]")
    # [21/11/2018] [10:43]
    (jj, mm, aaaa) = dateGetResult.split('/')
    (hh, ii) = timeGetResult.split(':')
    # print("[" + aaaa + "][" + mm + "][" + jj + "][" + hh + "][" + ii + "]")
    # [2018][11][21][10][48]
else :
    try :    
        datetimeGetCmd = 'sudo hwclock -r'
        datetimeGetResult = os.popen(datetimeGetCmd).read()[:-1]
        
        # TODO
        # Le retour de la commande peut merder (pas de composant RTC, ...)
    except :
        jj = '18'
        mm = '03'
        aaaa = '2021'
        hh = '23'
        ii = '59'

# jj = '14'
# mm = '07'
# aaaa = '2019'
# hh = '23'
# ii = '59'

##  Gestion des zeros non significatifs.
l12 = list()
l24 = list()
l31 = list()
l60 = list()
for i in range(0, 13) :
    ii = str(i).zfill(2)
    l12.append(ii)
    l24.append(ii)
    l31.append(ii)
    l60.append(ii)
for i in range(13, 24) :
    # ii = str(i).zfill(2)
    l24.append(i)
    l31.append(i)
    l60.append(i)
for i in range(24, 32) :
    # ii = str(i).zfill(2)
    l31.append(i)
    l60.append(i)
for i in range(32, 61) :
    # ii = str(i).zfill(2)
    l60.append(i)

# Création de la fenêtre principale (main window)
w = Tk()
## Taille des caracteres 
w.option_add( "*font", "lucida 14 bold italic" )


# default_font = tkFont.nametofont("TkDefaultFont")
# default_font.configure(size=48)

w.title("Mise a l'heure de la framboise")
w.geometry('800x300+400+400')
w['bg'] = 'bisque'

FrameTitle = Frame(w, borderwidth=2, relief=GROOVE, bg="white")
FrameTitle.pack(side=TOP, padx=5, pady=5)
LabelTitle = Label(FrameTitle, text="Mise a l'heure", fg="navy")
LabelTitle.pack(padx=5, pady=5)#side=LEFT, 

FrameDate = Frame(w, borderwidth=2, relief=GROOVE)
FrameDate.pack(padx=10,pady=10)

FrameTime = Frame(w, borderwidth=2, relief=GROOVE)
FrameTime.pack(padx=10,pady=10)#side=LEFT,

FrameAction = Frame(w, borderwidth=2, relief=GROOVE)
FrameAction.pack(padx=5, pady=5)#side=BOTTOM, 

    
##  Annee
vYear = StringVar(FrameDate)
wYear = Spinbox(master=FrameDate, width_=4, from_=2017, to_=2024, textvariable=vYear)
wYear.pack(side = LEFT, padx = 5, pady = 5)
if (aaaa.isnumeric() and int(aaaa) > 1970) :
    vYear.set(aaaa)
else :
    vYear.set("2019")

##  Mois
vMonth = StringVar(FrameDate)
# wMonth = Spinbox(master=FrameDate, width_=2, from_=1, to_=12, textvariable=vMonth)
wMonth = Spinbox(master=FrameDate, width_=2, values=l12, textvariable=vMonth)
wMonth.pack(side = LEFT, padx = 5, pady = 5)
if (mm.isnumeric() and int(mm) > 0 and int(mm) < 13) :
    vMonth.set(mm)
else :
    vMonth.set("07")

##  Jour
vDay = StringVar(FrameDate)
wDay = Spinbox(master=FrameDate, width_=2, values=l31, textvariable=vDay)
wDay.pack(side = LEFT, padx = 5, pady = 5)
if (jj.isnumeric() and int(jj) > 0 and int(jj) < 32) :
    vDay.set(jj)
else :
    vDay.set("14")


##  Heure
vHour = StringVar(FrameDate)
wHour = Spinbox(master=FrameTime, width_=2, values=l24, textvariable=vHour)
wHour.pack(side = LEFT, padx = 5, pady = 5)
if (hh.isnumeric() and int(hh) > 0 and int(hh) < 24) :
    vHour.set(hh)
else :
    vHour.set("14")

##  Minutes
vMinute = StringVar(FrameDate)
wMinute = Spinbox(master=FrameTime, width_=2, values=l60, textvariable=vMinute)
wMinute.pack(side = LEFT, padx = 5, pady = 5)
if (ii.isnumeric() and int(ii) > 0 and int(ii) < 59) :
    vMinute.set(ii)
else :
    vMinute.set("00")

##  Secondes
vSecond = StringVar(FrameDate)
wSecond = Spinbox(master=FrameTime, width_=2, values=l60, textvariable=vSecond)
wSecond.pack(side = LEFT, padx = 5, pady = 5)
vSecond.set("00")


# Création d'un widget Button (bouton Lancer)
# BoutonLancer = Button(FrameAction, text="Mettre à l'heure", command=NouveauLance)
BoutonLancer = Button(FrameAction, text="Mettre à l'heure", command=SetDateTime)
# Positionnement du widget avec la méthode pack()
BoutonLancer.pack(side = LEFT, padx = 5, pady = 5)

# Création d'un widget Label (texte 'Résultat -> x')
Texte = StringVar()
LabelResultat = Label(FrameAction, textvariable = Texte, fg ='red', bg ='white')
LabelResultat.pack(side = LEFT, padx = 5, pady = 5)

# Création d'un widget Button (bouton Quitter)
BoutonQuitter = Button(FrameAction, text='Quitter', command=w.destroy)
BoutonQuitter.pack(side = LEFT, padx = 5, pady = 5)

# NouveauLance()


w.mainloop()