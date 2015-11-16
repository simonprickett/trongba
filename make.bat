path=%PATH%;C:\gbadev\tools\devkitadv\bin

gcc  -o tron.elf tron.c
objcopy -O binary tron.elf tron.gba
