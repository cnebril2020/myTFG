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

✅ -> Respuestas en [IAanswers.md](AIanswers.md).
❓ -> Posibles preguntas para Enrique (hacer selección). 

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
8. ❓ ¿Diferencias y similitudes entre *ftrace*, *kprobe* y *eBPF*? [¹]
9. ❓ ¿Cuál es la parte negativa de usar *ftrace* para hacer *hijacking* de las llamadas al sistema?

[6 de abril]
10. ❓ ¿Cuándo nos enseñó Enrique a ocultar archivos de directorios, qué usaba? ¿*ftrace*, *kprobes*, *eBPF*?

[8 de abril]
11. ❓ He entendido más o menos en qué consisten *ftrace* y *kprobe* pero, ***eBPF*** no lo acabo de entender muy bien.
12. ❓ ¿En qué consiste *sysfs* y por qué es tan bueno para detectar *rootkits*?
13. ❓ Si realmente *sysfs* refleja los módulos cargados en el kernel, ¿por qué no leer (al menos como primer filtro), directamente de este archivo?
14. ❓ Si, una de las razones por la que *rkhunter* y *chkrootkit* son "malos" es porque tienen una base de datos 'estática' y los *rootkits* pueden cambiar su comportamiento y evadir estos mecanismos de detección, ¿qué diferencia habría con hacer una herramienta que verifique ciertos archivos del *sysfs*? Si se listan determinados *rootkits*, habríamos detectado un LKM malicioso.

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
- Virtual File System (VFS) hooking: manipular funciones del sistema de archivos (que no se muestren todas las entradas de archivos en un ```ls```, por ejemplo) 

Métodos más modernos:

- Uso ***ftrace***

    - Herramienta nativa para el trazado interno del kernel y ayudar a los desarrolladores para saber qué está pasando dentro del mismo. Su uso se pensó para usar *callbacks* al principio de las funciones del kernel y poder trazar el flujo de ejecución. Estos *callbacks* se pueden usar para hacer *hooking calls*.

    - Un *callback* es una *función* que se pasa como parámetro a otra **función** y, cuando se complete una tarea en la **función** original, salta el *callback* como una especie de aviso para conocer el flujo de ejecución.


- Uso ***kprobe***

    - Es una especie de debugger del kernel. Puedes poner puntos de interrupción en la ejecución del kernel y conocer/modificar el valor de ciertos parámetros o, incluso, cambiar el flujo de ejecución del kernel. 

    - ***ketprobe*** es parecido a *kprobe* pero, específicamente diseñado para obtener el valor de funciones con **valor de retorno**. 

[¹]: Diferencias ***ftrace*** y ***kprobe***:

    - [ftrace&kprobes_diferences.md](ftrace&kprobes_diferences.md)

- Uso **eBPF** (Extended Berkeley Packet Filter): extended from original BPF (Berkeley Packet Filter)

    - Herramienta muy potente y versátil. Preguntar a Enrique en qué consiste, mas o menos.

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
 
## 3. LKM rootkit detection

Entramos en el área de interés de este TFG. Intentar detectar, o incluso mitigar, el *rootkit*.

> Detecting an LKM rootkit is very difficult, and mitigation is even more complex. —*Matheuz*—

Herramientas antiguas y obsoletas:

1. *rkhunter*: es *'signature-based'*, es decir, se basa en comparar patrones y códigos maliciosos de otros *rootkits* conocidos y, en base a eso, detecta amenazas. El panorama actual ha evolucionado mucho y las técnicas de ocultación de los *rootkits* cambia de forma muy dinámica, no se puede depender de una base de datos estática.
2. *chkrootkit*: usa el mismo método, igual de obsoleto.

Si vamos al archivo ```/var/log/rkhunter.log```, podemos observar las firmas con las que intecta detectar los *rootkits*, sabiendo esto el método de *bypasear* la detección se vuelve trivial (le cambias el nombre a las funciones). Por ejemplo, en el caso del *rootkit* ```Diamorphine LKM```, busca estas funciones:

```bash
[11:26:53] Checking for Diamorphine LKM...
[11:26:53]   Checking for kernel symbol 'diamorphine'        [ Not found ]
[11:26:54]   Checking for kernel symbol 'module_hide'        [ Not found ]
[11:26:54]   Checking for kernel symbol 'module_hidden'      [ Not found ]
[11:26:54]   Checking for kernel symbol 'is_invisible'       [ Not found ]
[11:26:54]   Checking for kernel symbol 'hacked_getdents'    [ Not found ]
[11:26:54]   Checking for kernel symbol 'hacked_kill'        [ Not found ]
[11:26:54] Diamorphine LKM                                   [ Not found ]
```

Herramientas MODERNAS para **detectar** *rootkits*:

1. *sysfs*: sistema de archivos virtuales que expone la estructura interna del kernel, sus módulos, dispositivos, controladores y objetos relacionados. Más info en [sysfs.md](sysfs.md). 

Aún así, algunos kernel como ```basilisk``` consiguen evadir ciertos "listados" de *sysfs* y, por ejemplo, si lo intentamos listar, no aparece (comportamiento de ocultación por parte del *rootkit*):

```bash
$> sudo insmod basilisk.ko
$> lsmod | grep basilisk
$> 
# En cambio, sus funciones si aparecen listadas
$> sudo cat /sys/kernel/tracing/available_filter_functions| grep basilisk
  is_bad_path [basilisk]
  crc32 [basilisk]
  resolve_filename [basilisk]
  read_hook [basilisk]
  hook_openat [basilisk]
  show_refcnt [basilisk]
  init_this_kobj [basilisk]
  fh_kprobe_lookup_name [basilisk]
  fh_install_hook [basilisk]
  fh_remove_hook [basilisk]
  fh_install_hooks [basilisk]
  fh_remove_hooks [basilisk]
  sig_handle [basilisk]
  hook_seq_read [basilisk]
  set_root [basilisk]
  h_lkm_protect [basilisk]
  h_lkm_hide [basilisk]
```

