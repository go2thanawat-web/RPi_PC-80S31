#!/bin/bash

cd /home/pi/8001

# collect disks except SDOS
games=()

for f in *.d88
do
    if [ "$f" != "SDOS.d88" ]; then
        games+=("$f")
    fi
done

count=${#games[@]}
index=0

if [ $count -eq 0 ]; then
    echo "No disk files found"
    exit
fi

while true
do

clear

# CRT GREEN
echo -e "\e[32m"

echo "################################################"
echo "#                                              #"
echo "#     NEC PC-8001 DISK DRIVE EMULATOR          #"
echo "#                                              #"
echo "################################################"
echo ""
echo " Drive A : SDOS.d88"
echo " Drive B : ${games[$index]}"
echo ""
echo " Disk $((index+1))/$count"
echo ""
echo " GPIO3 = NEXT DISK"
echo " GPIO2 = PREVIOUS DISK"
echo ""
echo " Waiting for button..."
echo ""

./pc80s31 SDOS.d88 "${games[$index]}" &
PID=$!

# wait button
while true
do

next=$(raspi-gpio get 3 | grep -o "level=[01]" | cut -d= -f2)
prev=$(raspi-gpio get 2 | grep -o "level=[01]" | cut -d= -f2)

echo -ne "GPIO3=$next  GPIO2=$prev   \r"

if [ "$next" = "0" ]; then
    echo ""
    echo "NEXT DISK SELECTED"
    index=$(( (index+1) % count ))
    break
fi

if [ "$prev" = "0" ]; then
    echo ""
    echo "PREVIOUS DISK SELECTED"
    index=$(( (index-1+count) % count ))
    break
fi

sleep 0.1

done

echo ""
echo "Stopping emulator..."

kill $PID 2>/dev/null
sleep 1

if ps -p $PID > /dev/null
then
    echo "Force kill emulator..."
    kill -9 $PID
fi

echo "Switching disk..."
sleep 1

done
