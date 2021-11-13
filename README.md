# SO-TP3
Utilizando la imagen de docker:

make all : para compilar server y client

make clean : para borrar ejecutables server y client

make test : para ejecutar analisis estatico de pvs y cpp

make cleantest : para borrar reportes de analisis estatico

Para utilizar analisis dinamico con valgrind correr desde la terminal

EE encontrados:

-DOG se puede ver utilizando cat server o strings server (tambien implementado en serverDOG)

-VALGRIND_ES_GENIAL utilizando : valgrind ./server 2>> results.valgrind (no implementado)
