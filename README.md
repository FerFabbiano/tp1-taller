# Taller de Programación - TP1

**Alumno**: Fernando Fabbiano

**Padrón**: 102464

**URL**: https://github.com/FerFabbiano/tp1

**Versionado Semántico**: https://semver.org/lang/es/

1. **Descripción del trabajo**

	Para encarar la resolución del trabajo práctico, mi mayor dificultad fue la de parsear el archivo que contiene las llamadas a los procedimientos remotos. En un comienzo, para resolver este tema, implementé una función que leía caracter a caracter el archivo, y cuando encontraba un espacio, almacenaba la palabra encontrada en un Struct. Luego, si encontraba un paréntesis de apertura, sabía que los parámetros habían comenzado, y realizaba el mismo procedimiento que antes, pero ahora buscando comas, en vez de espacios. Al cabo de unos días, y de leer un intercambio de mensajes en el foro de la materia, entendí que esta no era una forma eficiente de recorrer el archivo, ya que la función fgetc hace uso de la syscall read, que es muy costosa. Además, había pensado en almacenar todos los datos del header y del body en un struct, y luego mandarlo a través del socket, pero investigando, me encontré con que esto podría traerme muchos problemas. 
	
	La solución que implementé para este problema consiste en una función que lee el archivo de a 32 bytes. Lo que leo, lo almaceno en un buffer dinámico, cuyo tamaño va creciendo de a 32 bytes con cada fread que hago al archivo en cuestión. Luego de cada lectura de 32 bytes del archivo, y de almacenar lo leído en el buffer dinámico, chequeo si tengo en él un salto de línea. Si lo encuentro, dejo de leer el archivo y paso a "depurar" lo leído. 

	Cuando esto sucede, puede suceder que haya leído bytes que pertenezcan al próximo llamado a un procedimiento, y que por tanto no me interesan analizar en ese momento. Entonces, ese buffer dinámico tiene que pasar por una "depuración" para poder obtener una array que solo contenga los datos de la línea que me interesa procesar. Es decir, debo borrar del array los bytes que leí de más. Para esto, implementé una función que va copiando en un nuevo buffer carcater a caracter lo que contiene el array "sucio" hasta que encuntro el salto de línea, y dejo de copiar. Ahora tengo los datos necesario para armar mi tira de bytes y poder enviarla a través del socket, pero el tintero del archivo, quedó en una posición que no es la que realmente quiero, ya que puede estar varios bytes por delante del salto de línea. Luego, con un fseek, sitúo el file pointer a la cantidad de bytes que tiene mi linea depurada. De esta forma, el próximo fread que haga para obtener la próxima línea, lo haré comenzando desde el inicio de la línea. 

	Del mismo modo, la manera en la que debía recibir los datos del lado del servidor me trajo complicaciones a la hora de pensarlo e imaginarmelo. Lo que hicé fue, desde el cliente, enviar la información en dos tandas (tres en caso de existir parámetros para ese llamado a procedimiento remoto). Asimismo, del lado del servidor, implementé dos (o tres) receive, cada uno correspondiente a cada envio del lado del cliente. Esto me peritió, con los primeros 16 bytes recibidos, conocer la información necesaria para poder desarrollar la desserialización. En estos bytes, los primeros 16 del header, tengo la información correspondiente a la longitud del array de opciones, la longitud del body (puedo determinar si existe tall body, o no). Incluso puedo saber e imprimir el id del mensaje. Una vez obtenidos estos tamaños, puedo crear los arreglos de carcateres con el tamaño necesario para hacer los siguientes receive, que contendrán el array de opciones y el body.

<p align="center">
<img src="https://user-images.githubusercontent.com/38590805/81076211-c5684a00-8ec1-11ea-86f9-80d3352d81a7.png" 			width=900 height=600>


1. **Aclaraciones**

	A la hora de iniciar el Trabajo Práctico, luego de ver la clase en la que se explicó el tema "Sockets", los conceptos parecían estar claros, y la forma en la que debía encarar la resolución del mismo también. Programar el TDA Socket no me presentó mayores dificultades. Siguiendo los consejos de la cátedra, consulté mucho la documentación sobre el tema utilizando la página "man7.org", y esto fue de gran ayuda. Pasados unos días, ya tenía un servidor y un cliente comunicándose a traves de un socket de mi autoría. A esa altura, creía tener el proyecto casi terminado. 

	A la hora de encarar la "segunda parte" del Trabajo Práctico, es decir, codear el protocolo, me puse a escribir líneas de código sin antes ponerme a pensar detenidamente e investigar que me convenía para desarrolarlo. Esto llevo a que pasara varios dias, varias horas, sentado frente a la computadora, creyendo que estaba avanzando, pero siempre llegando a un punto en el que me encontraba sin entender que, y para que era lo que estaba codeando. Cuando me di cuenta de esto, decidí que primero era mejor entender a fondo como era que funcionaba el protocolo D-bus (me refiero a entender, al menos, la versión resumida de la implementación del protocolo que nos fue otorgado en la consigna), y hacer un seguimiento con papel y lápiz del "Ejemplo de uso real" dado en la consigna. Para cuando me di cuenta de que esto era necesario para encarar la resolución, me encontré con que había perdido mucho tiempo sin saber que es lo que estaba codeando, y por ende los días habían pasado y aún no habia avanzado mucho, por lo que el tiempo empezó también a jugarme encontra. Desde mi punto de vista, ese fue mi mayor problema a la hora de realizar este Trabajo, y no quería dejar de comentarlo en este informe. 
	
