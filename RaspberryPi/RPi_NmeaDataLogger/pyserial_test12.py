import os, glob
import serial
import serial.tools.list_ports
# print [port for port in serial.tools.list_ports.comports() if port[2] != 'n/a']
listPorts = serial.tools.list_ports.comports()
# for p in listPorts :
    # print(p)
##  COM3 - Intel(R) Active Management Technology - SOL (COM3)

# conserver le r devant " pour bonne gestion de l'antislash
fileDir = r"c:\temp"
filePrefix = "rs232log_"
fileSuffix = ".nmea"
fileNumber = 0
fileStrNum = ""
filePrefixFull = os.path.join(fileDir, filePrefix)
filePrefixI = fileSuffixI = None

listLogPorts  = list()
listWishPorts = list()
listWishPorts.append('COM34')

portName = 'COM3'
portSpeed = 4800

##  Le point de montage (clef USB ?) existe t-il ?
if (os.path.isdir(fileDir)) :
    # print("Le fileDir [" + fileDir + "] existe")
    pass
else :    
    print("Le fileDir [" + fileDir + "] n'existe pas")
    quit(1)

##  Quel est le dernier numero de fichier utilise ?
filesExistingList = glob.glob(os.path.join(fileDir, filePrefix + "*" + fileSuffix))
# print(filesExistingList)
if (len(filesExistingList) == 0) :
    fileNumber = 0
    fileStrNum = "00000000"
else :
    filesExistingList.sort(reverse=True)
    # print(filesExistingList)
    # Quel nom de fichier est numerique ?
    for f in filesExistingList :
        f = os.path.basename(f)
        filePrefixI = f.find(filePrefix) + len(filePrefix)
        fileSuffixI = f.rfind(fileSuffix)
        fNumPart = f[filePrefixI:fileSuffixI]
        # print("f:", f)
        # print("prefix:", filePrefixI, " suffix:", fileSuffixI)
        # print("?:[" + fNumPart + "]")
        # Test si cette partie du nom de fichier est numerique.
        # Si OUI, alors incrément de 1
        if (fNumPart.isnumeric()) :
            fileNumber = int(fNumPart) + 1
            # formatage avec zeros non significatifs
            # Python 2.7 ? fileStrNum.zfill(fileNumber, 8)
            fileStrNum = str(fileNumber).zfill(8)
            break
        
# print("fileNumber:", fileNumber, ", fileStrNum:", fileStrNum)

##  Essai de creation du fichier
##  https://www.novixys.com/blog/python-check-file-can-read-write/
##  https://stackoverflow.com/questions/2113427/determining-whether-a-directory-is-writeable
try:
    objFile = open(filePrefixFull + fileStrNum + fileSuffix, 'w')
    print("Creation de [" + filePrefixFull + fileStrNum + fileSuffix + "]")
except OsError:
    print("! Pb a la creation de [" + filePrefixFull + fileStrNum + fileSuffix + "]")
    quit(2)

## Ecriture de quelques informations dans le fichiers ...
objFile.write("## Fichier " + filePrefixFull + fileStrNum + fileSuffix + "\n")

## Ecriture de la liste des ports detectes
objFile.write("##  Liste des ports :\n")
for i, p in enumerate(listPorts) :
    objFile.write("##  " + str(i) + "\t" + str(p[0]) + "\t" + str(p[1]) + "\t" + str(p[2]) + "\n")
    if (str(p[0]) in listWishPorts) :
        listLogPorts.append(str(p[0]))
        print(p[0] + " est ajoute a la liste des ports serie utilises")
        objFile.write("##  " + p[0] + " est ajoute a la liste des ports serie utilises\n")
if (len(listLogPorts) == 0) :
    print("! Pas de port serie voulu et existant.")
    objFile.write("##   Pas de port serie voulu et existant.\n")
    objFile.close()
    quit(3)
    
# port = serial.Serial(portName, portSpeed, timeout=3)
# print(port.readline())
# port.close()

objFile.close()
quit(0)

