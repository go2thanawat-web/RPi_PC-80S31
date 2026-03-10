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
    echo "No game disks found"
    exit
fi

while true
do

clear
echo -e "\e[32m"

echo "################################################"
echo "#     NEC PC-8001 DISK DRIVE EMULATOR         #"
echo "################################################"
echo ""
echo " Drive A : SDOS.d88"
echo " Drive B : ${games[$index]}"
echo ""
echo " Games found : $count"
echo ""
echo " DEBUG: waiting GPIO3 button..."
echo ""

./pc80s31 SDOS.d88 "${games[$index]}" &
PID=$!

# wait button
while true
do

    state=$(raspi-gpio get 3 | grep -o "level=[01]" | cut -d= -f2)

    echo -ne "GPIO3 level = $state   \r"

    if [ "$state" = "0" ]; then
        echo ""
        echo "BUTTON DETECTED!"
        break
    fi

    sleep 0.1

done

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
index=$(( (index+1) % count ))

done
