#! /bin/sh
#first param:binary name
#last param:copy folder

cp -v  $(ldd $1 | awk '{if (match($3,"/")){ printf("%s "),$3 } }') $2

