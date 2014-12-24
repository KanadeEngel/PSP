PATH = c:\pspsdk\bin;
make clean
make
rm *.prx *.o *.elf *.sfo
cd ipl_dump_prx

make clean
make
rm *.o *.elf
pause
