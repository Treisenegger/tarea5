# Server (Instrucciones para Linux [Ubuntu])

Para compilar y ejecutar el servidor en un comando se debe abrir la terminal en la ruta de la carpeta server y ejecutar:

	g++ -std=c++14 server.cpp -o server.out && ./server.out

Si se desea compilar y luego ejecutar por separado se debe, nuevamente, abrir la terminal en la ruta de la carpeta server y ejecutar:

	g++ -std=c++14 server.cpp -w -o server.out
	./server.out

Para terminar la ejecución del código se debe apretar Ctrl+C.
