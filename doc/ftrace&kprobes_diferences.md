### Comparación de **kprobes** vs **ftrace** en el Contexto de un **Rootkit**

#### **1. Propósito de kprobes y ftrace**

**kprobes: Propósito**
- El propósito principal de **kprobes** es **interrumpir y modificar** el comportamiento de funciones críticas del kernel, como **`setuid()`** o **`sys_read()`**, permitiendo **alterar el flujo de ejecución** del sistema.
- **Uso en Rootkit:** Se utiliza para **elevar privilegios de un proceso** (por ejemplo, cambiando el UID de un proceso a 0, elevándolo a root) o para **modificar la ejecución del kernel** a favor de actividades maliciosas.

**Ejemplo práctico con kprobes:**
Imagina que un atacante quiere **elevar privilegios a root**. El atacante puede usar un **kprobe** para engancharse en la función `__x64_sys_setuid`, que gestiona las llamadas a `setuid(0)`, y en el **post-handler**, modificar las credenciales del proceso para otorgarle privilegios de root.

---

**ftrace: Propósito**
- **ftrace** es una herramienta diseñada para **monitorear** y **trazar** el comportamiento de funciones del kernel sin alterar directamente su flujo de ejecución.
- **Uso en Rootkit:** Se utiliza para **ocultar actividades** como conexiones de red maliciosas o procesos específicos sin alterar el comportamiento subyacente del kernel.

**Ejemplo práctico con ftrace:**
Imagina que un atacante quiere **ocultar conexiones de red** del rootkit en un puerto específico (por ejemplo, puerto 8081). Con **ftrace**, puede engancharse en la función que lista las conexiones TCP (como `tcp4_seq_show`) y **filtrar** aquellas conexiones que corresponden al puerto que está utilizando el rootkit.

---

### **2. Diferencias Clave Entre kprobes y ftrace en un Rootkit**

| **Característica**                    | **kprobes (rootkit)**                                | **ftrace (rootkit)**                                |
|---------------------------------------|------------------------------------------------------|----------------------------------------------------|
| **Propósito**                         | Interceptar y **modificar el comportamiento** del kernel (como elevar privilegios). | **Monitorear y ocultar** actividades sin alterar el flujo del kernel. |
| **Modificación del sistema**          | Modifica directamente el comportamiento de las funciones del kernel. | No modifica las funciones del kernel, solo cambia la salida del sistema. |
| **Impacto en el sistema**             | Más invasivo, puede afectar el rendimiento y la estabilidad. | Menos invasivo, tiene un impacto menor en el rendimiento. |
| **Uso en rootkits**                   | Eleva privilegios de un proceso, cambia credenciales o modifica directamente el flujo de ejecución. | Oculta actividades (como conexiones de red) sin modificar el sistema subyacente. |
| **Detectabilidad**                    | Puede ser detectado por un análisis profundo del kernel, ya que modifica funciones críticas. | Es más difícil de detectar porque solo altera la visibilidad de la salida del sistema. |

---

### **Conclusión**:

- **kprobes** es **más adecuado** cuando se necesita **modificar** directamente el comportamiento del kernel o **cambiar las credenciales** de un proceso (como elevar privilegios o modificar la ejecución de una función). Es más **intrusivo** y tiene un **impacto mayor** en el sistema.
  
- **ftrace**, por otro lado, es **ideal** cuando se quiere **ocultar actividades** del sistema (como conexiones maliciosas) sin cambiar las funciones del kernel o su comportamiento. Es menos **intrusivo** y más adecuado para **evitar la detección** sin alterar el sistema, pero no permite **modificar directamente** el comportamiento del kernel (como elevar privilegios de un proceso).

En resumen, **kprobes** es útil para manipular el sistema de manera profunda, mientras que **ftrace** es más adecuado para **ocultar** actividades sin afectar el comportamiento subyacente del sistema.

