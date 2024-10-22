# Threads

# Aclaracion sobre Queue:
La clase Queue utilizada en este trabajo es la clase template que está en el directorio libs del hands-on-threads. Decidí que sería una BoundedQueue, ya que considere que es mejor tener que esperar a algún cliente más lento a perder información. Tomé como valor máximo de mensajes posibles el número 30, ya que me pareció un número lo suficientemente grande como para no generar que se bloqueen las queues constantemente, pero no tan grande como para que básicamente se convierta en una UnboundedQueue.

# Licencia:
GPL v2


# Codigo no propio: 
En este TP se usan archivos y/o librerias que no son de mi autoria. En particular, las implementaciones de Socket, Thread y Queue son sacadas de 2 repositorios de Github de Martin Di Paola. En particular, la implementacion de Queue fue modificada un poco para adaptarse a este trabajo. 

Los archivos que no me pertenecen y se usan en este trabajo son:

- socket.cpp
- socket.h
- resolver.cpp
- resolver.h
- resolvererror.cpp
- resolvererror.h
- liberror.cpp
- liberror.h
- thread.h
- queue.h

Los repositorios en dónde se pueden encontrar todos estos archivos son:
- Threads: https://github.com/eldipa/hands-on-threads.git
- Sockets: https://github.com/eldipa/hands-on-sockets-in-cpp.git

