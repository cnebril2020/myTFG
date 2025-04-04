---
title: "The Art Of Linux Kernel Rootkit"
---

# The Art Of Linux Kernel Rootkit

Apuntes sucios sobre el artículo "The Art of Linux Kernel Rootkit" de tmp.0ut.
[https://tmpout.sh/4/10.html](https://tmpout.sh/4/10.html)

## REFERENCIAS (muy importante)

1. Lista de números de llamadas al sistema: [https://filippo.io/linux-syscall-table/](https://filippo.io/linux-syscall-table/)
2. Cómo funciona la creación de un *rootkit* en *userland*: [https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/)
3. Creación de LKM *rootkits*:
    1. [https://xcellerator.github.io/tags/rootkit/](https://xcellerator.github.io/tags/rootkit/)
    2. [https://blog.convisoappsec.com/linux-rootkits-hooking-syscalls/](https://blog.convisoappsec.com/linux-rootkits-hooking-syscalls/)
    3. [http://www.ouah.org/LKM_HACKING.html](http://www.ouah.org/LKM_HACKING.html)
4. 

## Preguntas Enrique/Claude [✅/❓]

Respuestas en [IAanswers.md](AIanswers.md) las marcadas con un tick verde.

[1 de abril 2025]
1. ✅ ¿Un *rootkit* de *kernel land* tiene que ser necesariamente un módulo de kernel cargable (LKM)?
2. ✅ ¿Cómo se prioriza la gestión de señales al kernel? Me refiero, ¿qué función tiene prioridad respecto a otra? Si creo un programa como `hook.c` y lo cargo en el kernel, ¿por qué se ejecuta esta función y no la función que originalmente gestionaba la señal `__x64_sys_kill`?
3. ✅ En `hook.c`, ¿por qué se usa la señal como *trigger* si originalmente hablabamos de `__x64_sys_kill`?
4. ✅ ¿Qué significan todos esos id's en `hook.c`?

[2 de abril 2025]
5. ❓ ¿Por qué se dejaron de usar técnicas de secuestro de llamadas al sistema o VFS hooking? En el artículo no entra mucho al detalle.
6. ❓ ¿Cuál es una fuente de conocimiento válida para aprender a programar en el ámbito del kernel?

[3 de abril 2025]
7. ❓ ¿Se podría decir que *kprobe* es una especie de debugger del kernel?

[4 de abril 2025]
8. ❓ Diferencias y similitudes entre *ftrace*, *kprobe* y *eBPF*



## 1. What is a rootkit?

LKM - Loadable Kernel Module (Módulo de kernel cargable)

Rootkits en *userland* más fáciles de detectar y mitigar (operan en espacio de usuario) que los de *kernel land*.

Técnica común para crear un rootkit en *userland* es el uso de **LD_PRELOAD**.

### ¿Cómo crear un *rootkit* en *userland*? 

Cambiando el comportamiento predeterminado del sistema para que se ajuste a los fines del atacante. También llamado ***hooking*** de las *syscalls*. 

Ejemplo: [https://github.com/cnebril2020/Rootkit/blob/main/Ring0/become_root/hook.c](https://github.com/cnebril2020/Rootkit/blob/main/Ring0/become_root/hook.c)

`hook.c` es un modulo de kernel que, al recibir la señal kill (en concreto la 59), se utiliza para ejecutar la función `SpawnRoot()` para elevar privilegios.

#### Diferencias entre módulos del kernel y programas normales

- Programas de espacio de usuario:

    1. Comienzan su ejecución desde la función main()
    2. Son ejecutados como procesos independientes
    3. Tienen un punto de entrada y salida claro

- Módulos del kernel:

    1. Son extensiones cargables del kernel Linux
    2. No se ejecutan como procesos separados, sino como parte del propio kernel
    3. Utilizan puntos de entrada y salida específicos definidos por las macros module_init() y module_exit()

## 2. Modern hooking techniques

Después de ver cómo se pueden secuestrar llamadas al sistema añadiendo módulos al kernel, vamos a aprender algunos métodos más modernos.

Métodos anticuados:

- Secuestrar llamadas al sistema añadiendo módulos de kernel maliciosos
- Virtual File System (VFS) hooking: manipular funciones 

Métodos más modernos:

- Uso ***ftrace***

    > Herramienta nativa para el trazado interno del kernel y ayudar a los desarrolladores para saber qué está pasando dentro del mismo. Su uso se pensó para usar *callbacks* al principio de las funciones del kernel y poder trazar el flujo de ejecución. Estos *callbacks* se pueden usar para hacer *hooking calls*.

    - Un *callback* es una *función* que se pasa como parámetro a otra **función** y, cuando se complete una tarea en la **función** original, salta el *callback* como una especie de aviso para conocer el flujo de ejecución.

- Uso ***kprobe***

    > Es una especie de debugger del kernel. Puedes poner puntos de interrupción en la ejecución del kernel y conocer/modificar el valor de ciertos parámetros o, incluso, cambiar el flujo de ejecución del kernel. 

    > ***ketprobe*** es parecido a *kprobe* pero, específicamente diseñado para obtener el valor de funciones con **valor de retorno**. 

- Uso **eBPF** (Extended Berkeley Packet Filter): extended from original BPF (Berkeley Packet Filter)

### Explicación más detallada de las técnicas de *hijacking* modernas

| Característica             | ftrace                                                                                                  | kprobe / kretprobe                                                                                          | eBPF                                                                                                     |
|----------------------------|---------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------|
| **Descripción General**    | Infraestructura nativa de trazado en el kernel, inicialmente diseñada para depuración y análisis.       | Permite insertar puntos de interrupción (probes) en funciones del kernel en tiempo real sin modificar código. | Framework extendido del BPF original que ejecuta código seguro en el kernel sin necesidad de módulos.    |
| **Ventajas**               | - Integrado en el kernel.<br>- Permite hook de funciones y live patching.<br>- Potente para trazado.      | - Alta flexibilidad al poder enganchar casi cualquier función.<br>- Uso de pre_handler y post_handler.<br>- No requiere recompilación del kernel. | - Bajo overhead y alta eficiencia.<br>- Ambiente sandbox (seguro).<br>- No carga módulos tradicionales.  |
| **Desventajas**            | - Puede impactar el rendimiento si se usa intensivamente.<br>- Requiere configuración y conocimientos específicos. | - Riesgo de inestabilidad si se engancha funciones críticas.<br>- Su actividad puede ser detectada mediante trazado. | - Curva de aprendizaje alta.<br>- Requiere herramientas específicas (clang/LLVM).                       |
| **Uso Típico**             | Hook de syscalls para modificar o rastrear llamadas, live patching y monitoreo interno del kernel.       | Depuración, monitoreo y modificación del comportamiento de funciones en tiempo real.                        | Monitoreo avanzado, seguridad, detección de anomalías y, en algunos casos, hooking sin cargar módulos.   |
| **Requerimientos**         | Kernel con ftrace habilitado, permisos de root y conocimientos en su configuración y API.              | Kernel que soporte kprobes, módulos cargables y permisos de root para insertar los probes.                    | Kernel moderno (Linux 4.x+), herramientas de compilación como clang/LLVM, bpftrace y permisos de root.    |
| **Nivel de Complejidad**   | Medio a alto, dependiendo de la función y del hook implementado.                                       | Medio; se requiere un buen entendimiento del funcionamiento interno del kernel y de la función a enganchar.   | Alto, especialmente en el desarrollo, compilación y depuración de programas eBPF.                        |
| **Impacto en el Sistema**  | Potencial impacto en performance si se abusa de los hooks o si se implementan de forma ineficiente.       | Puede afectar la estabilidad del sistema si se engancha incorrectamente funciones críticas.                   | Diseñado para minimizar overhead, aunque un mal programado puede ocasionar problemas.                    |
| **Posibilidad de Detección** | Las alteraciones en la infraestructura de trazado (por ejemplo, en las tablas de funciones) pueden ser detectadas. | La presencia de probes y sus registros en el sistema (por ejemplo, en logs o trazas) puede revelar su uso.     | Es más discreto y difícil de detectar sin herramientas especializadas, aunque no es completamente invisible. |

## 

