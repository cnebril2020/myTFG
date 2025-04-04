# TFG

Devolpment for a tool capable of detenting rootkits making direct systemcalls to the kernel and comparing them to the usual output. I will give a name for the tool and imporove this description. I promise.

# Apuntes diarios (cuaderno de bitácora)

## 27 de marzo 2025

Empieza mi aventura con el TFG. He hablado con Enrique en el día de hoy sobre este nuevo proyecto. La idea es presentarlo el año que viene sobre estas fechas, antes de que acabe el curso e idealmente antes de mayo-junio.

El primer paso es tener claro en qué consiste el proyecto (escuchar audio tutoría). 

El segundo leerme "The Art of Linux Kernel Rootkit" de tmp.0ut

El tercero implementar el MVP que me ha descrito en el día de hoy: hacer la llamada al sistema de 'ls' a pelo y comparar con lo que me da el area de usuario.

Varias preguntas que me surgen antes de empezar

- ¿Surge algún problema o modificación si las herramientas de GNU migran a Rust?
- No tengo ninguna más, no sé por qué he puesto "varias".

---

## 30 de marzo 2025

Llevo un par de días sin pasarme por aquí, no va a ser una costumbre. 

Hoy me he propuesto intentar no hacer del TFG una tortura para mi propia salud mental. ¿En qué consiste este concepto? Me niego a investigar ABSOLUTAMENTE todo antes de empezar y, después, desarrollar la herramienta. 

Esto es algo totalmente práctico.
Se aprende haciendo.
No te confundas.
Hazme caso.

Asi que, dicho esto, hoy voy a seguir los puntos descritos en el día 27 de marzo y, al acabar de contextualizar el TFG, ponerme a hacerlo. Y punto.

Hoy quiero empezarlo, asi que ponte las pilas. 

Me va a llevar más tiempo del esperado leerme el artículo al completo. Necesito comprender bastante más el contexto del TFG y, sin duda, esto me va a ayudar.

Seguimos.

# 1 de abril 2025

Ya me he puesto a aprender de verdad de qué va esto y, la verdad que me está gustando, asi que no hay excusa.

Mañana, o esta misma tarde, seguiré con ello. 

Muy interesante.

# 2 de abril 2025

Tienen todos los ejemplos subidos, con todos los archivos. La verdad que un alivio, siendo sinceros. Pensaba ya que me iba a tocar completar los archivos restantes con ChatGPT o algo así, y no me gusta mucho no ser fiel al original.

Me van a servir mucho.

La comunidad de Discord también, tienen hasta trucos sobre cómo detectar rootkits. Seguro que me viene bien en un futuro.

Poco a poco, pero con buena letra.

Me ha quedado por comprender el segundo programa de ejemplo del artículo `netstat.c` en el que, con la ayuda de *ftrace*, se consigue ocultar la información de un proceso de red en el puerto 8081 usando los comandos `netstat` y `lsof`

# 3 de abril 2025

Me estoy instalando una máquina virtual nueva en Virtual Box para hacer las pruebas. Casi no lo hago pero sé que es lo correcto, por si acaso.

He medio entendido el programa, aunque desconozco mucho por ahora del mundo del *kernel*.

Creo que por hoy, con probar ```netstat.c``` en la máquina virtual, es suficiente. Me ha llevado más tiempo del esperado.

# 4 de abril 2025


