Installation de Raspbian "Buster"



Clavier NumLock au démarrage

https://raspberrypi.stackexchange.com/questions/38794/enable-num-lock-at-boot-raspberry-pi 

While this isn't directly answering the Raspbian Jessie question, for those other Googler's who run Raspbian Stretch, which doesn't have /etc/kbd/config, here's the solution:

    Install numlockx: sudo apt-get install numlockx
    Edit this file: sudo nano /usr/share/lightdm/lightdm.conf.d/01_debian.conf
    At the end of that file, add this line: greeter-setup-script=/usr/bin/numlockx on
    Save the file and reboot, and you NumLock key should still be on

Avant : 
	Open up /etc/kbd/config with your favorite editor.
	Uncomment LEDS=+num