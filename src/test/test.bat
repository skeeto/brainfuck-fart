gcc -Os -s -Wall -Werror -fstack-protector -std=c99 -pedantic-errors -o .\testing.exe "%1.c"
.\testing.exe
del .\testing.exe
