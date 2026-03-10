# use Raspberry Pi as  PC-80S31 disk   ใช้ raspbery pi เป็น Disk ของ PC8001

## pin connector 
```
FDD Port(5V)           Raspberry Pi(3.3V)
Pin 1- 8 (PB0-PB7) --> GPIO4-11 * reduce voltage form 5 to 3.3v by level shiffer
Pin19-26 (PA0-PA7) <-- GPIO12-19
Pin27-30 (PC4-7)   --> GPIO20-23 * reduce voltage form 5 to 3.3v by level shiffer
Pin31-33 (PC0-2)   <-- GPIO24-26
   Pin35 (RESET#)  --> GPIO27 * reduce voltage form 5 to 3.3v by level shiffer
   Pin36 (GND)     --- GND

connect pin nummber  9 and 10 of FDD 
FDD Port         FDD Port
Pin9 (EXTON) --- Pin10 (GND)
```
if you want to save your time use my pcb istead

## how to run code
```
git clone https://github.com/go2thanawat-web/RPi_PC-80S31.git
cd RPi_PC-80S31
$ make
```

## start.d88
`
$ touch blank.d88
$ sudo ./pc80s31 SDOS.d88 block.d88
```

