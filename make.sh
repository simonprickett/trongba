rm tron.o
rm tron.elf
rm tron.gba

arm-none-eabi-gcc -mthumb -mthumb-interwork -c tron.c
arm-none-eabi-gcc -specs=gba.specs -mthumb -mthumb-interwork tron.o -o tron.elf
arm-none-eabi-objcopy -O binary tron.elf tron.gba
gbafix tron.gba
