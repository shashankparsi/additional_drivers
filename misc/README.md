how to use and test the misc driver:

step1:
insert the driver using the command : sudo insmod ./misc.ko

step2: 
observe the dmesg ring kernel buffer for the insertion message of misc-driver : dmesg

step 3:
in /dev directory you can find the character driver as misc-test-driver : ls /dev/misc-test-driver

step4: 
write the data to the driver:
echo "hello linux" > /dev/misc-test-driver

step5: 
read the data from driver
cat > /dev/misc-test-driver

additionally i have created a userspace app to read and write
./userapp
