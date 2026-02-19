# 📖 saburou-platform: Architecture Decision Records (ADR)

Este documento registra las decisiones técnicas y de mantenimiento de saburou-platform. Estas reglas
**priorizan al usuario**, que saburou-platform sea potente, fácilmente utilizable y que no deje de funcionar de
un día para otro.

---

## 🛠️ Nivel 1: Uso y Filosofía de Código

### (ADR-1) Estructura Física e Integridad (ODR Guard)

Wed 2026-02-18 12:30:44 v1

- **Decisión**: Cada versión mayor de la API reside en su propia carpeta física (ej. `v1/`) con su respectivo
  archivo paraguas (`v1.hpp`).
- **Sincronización**: Si el comportamiento interno cambia pero la API se mantiene, se incrementa la macro
  `SABUROU_PLATFORM_VX_CORE_REVISION`.
- **Protección**: Se utilizan centinelas de revisión que disparan un `#error` si se intentan mezclar diferentes
  revisiones del núcleo en una misma unidad de traducción.
- **Impacto**: Aislamiento total entre versiones. Se garantiza que el linker no produzca comportamientos
  híbridos o violaciones de la Regla de Definición Única (ODR).

### (ADR-2) Saneamiento del Entorno (Macro Cleanup)

Wed 2026-02-18 12:30:44 v1

- **Contexto**: Macros predefinidas por compiladores antiguos (como `linux` o `unix`) contaminan el espacio
  global y rompen enums o namespaces modernos.
- **Decisión**: saburou-platform aplica un `#undef` a estas macros por defecto al cargar el núcleo.
- **Impacto**: Permite utilizar nombres limpios y modernos como `os::type_t::linux`. Si el usuario requiere
  estas macros de vuelta para código heredado, se provee el cabecero `env/recover.hpp`.

### (ADR-3) Filosofía del 99% y Válvulas de Escape

Wed 2026-02-18 12:30:44 v1

- **Decisión**: La librería se diseña para cubrir de forma elegante y automática el 99% de los casos de uso
  comunes en detección de plataformas.
- **Overrides**: No se bloquea al usuario en el 1% restante. Se en medida de lo posible (mientras sea viable y
  tenga sentido) las versiones implementan _overrides_ manuales (como definir `SABUROU_PLATFORM_DEFINED = 1`)
  para desactivar detección. Esto implica que habrá que definir ciertas directivas a mano bajo ciertas
  compilaciones con detección manual por parte del usuario + posibles roturas de features, pero al menos se
  otorga alguna posibilidad al usuario durante la evolución del proyecto.

### (ADR-4) Evolución del Estándar C++

Wed 2026-02-18 12:30:44 v1

- **Decisión**: El estándar C++ requerido por saburou-platform es una decisión pragmática basada en la utilidad
  de las características del lenguaje, no un dogma inamovible.
- **Impacto**: Aunque actualmente se prioriza C++23 por potencia y ergonomía, si en el futuro es más
  beneficioso decrementar la versión para ganar alcance o subirla para aprovechar nuevas herramientas, se hará
  de forma comunicada.

---

## 🛡️ Nivel 2: Mantenimiento y Seguridad

### (ADR-5) Compromiso de Seguridad y Comunidad

Wed 2026-02-18 12:30:44 v1

- **Decisión**: Ante la detección de un fallo de seguridad crítico, el compromiso de saburou-platform es actuar
  con la máxima celeridad posible, dándole prioridad absoluta sobre el desarrollo de nuevas características.
- **Realismo**: La resolución efectiva dependerá de la complejidad técnica y el conocimiento disponible. **Se
  fomenta activamente la colaboración** de la comunidad para auditar, reportar y parchear fallos de forma
  **transparente** y profesional.

### (ADR-6) Registro de Cambios in-situ (Changelog)

Wed 2026-02-18 12:30:44 v1

- **Decisión**: Cada versión mantiene su historial de cambios con respecto a la versión anterior, así como los
  cambios del nucleo en un archivo físico situado en `vX/changelog.txt`.

### (ADR-7) Ramas de Especialización

Wed 2026-02-18 12:30:44 v1

- **Decisión**: saburou-platform no descarta la creación de ramas dedicadas o proyectos derivados para dar
  soporte a sistemas antiguos o estándares de C++ legacy (ej. C++11) si existe una necesidad real. Estas
  derivaciones se gestionarán con sus propias tablas de versiones.

---

## ⏳ Nivel 3: Ciclo de Vida y Retirada

### (ADR-10) Publicación Directa y Estabilidad Productiva

Thu 2026-02-19 20:10:00 v1

- **Supercede a**: [(ADR-8) Fase Alpha de Validación](#adr-8-fase-alpha-de-validación)
- **Decisión**: Se elimina la obligatoriedad del periodo de validación alpha (sufijo `-alpha`). Las versiones
  mayores pueden publicarse como `stable` de forma inmediata.
- **Razón**: El bloqueo de 15-60 días resulta incompatible con la necesidad de despliegue en producción de
  mejoras críticas de diseño. La estabilidad se garantiza mediante el aislamiento físico de la **(ADR-1)**.
- **Impacto**: Permite la evolución ágil del proyecto sin comprometer la integridad de versiones anteriores.

### (ADR-9) Soporte y Transición a Legacy-Archive

Wed 2026-02-18 12:30:44 v1

- **Fin de Soporte**: Una versión deja de recibir parches o mantenimiento activo cuando se cumplen **AMBAS**
  condiciones:
    1. Existen **3 versiones mayores** posteriores disponibles (Regla N-3).
    2. Han pasado al menos **6 meses** desde que la versión alcanzó el estado anterior.
- **Legacy-Archive**: Una versión se mueve físicamente a la carpeta de archivo (y se elimina de la rama
  principal) **2 años después** de haber alcanzado su "Fin de Soporte".
- **Impacto**:
    - Se garantiza un ciclo de vida predecible y extendido, protegiendo a los usuarios de obsolescencia
      prematura incluso ante lanzamientos frecuentes de nuevas versiones.
    - Habrá epocas en las que estaremos arrastrando código muy antiguo y la cantidad de archivos en el proyecto
      se vuelva absurdamente grande.

---

## 📜 Historial de Decisiones Supercedidas

### (ADR-8) Fase Alpha de Validación

Wed 2026-02-18 12:30:44 v1

- **Supercedido por**:
  [(ADR-10) Publicación Directa y Estabilidad Productiva](#adr-10-publicación-directa-y-estabilidad-productiva)
- **Decisión**: Toda nueva versión mayor de la API se publicará bajo el sufijo `-alpha` (ej. `v2-alpha`)
  durante al menos un periodo entre 15 y 60 días.
- **Objetivo**: Validar el diseño en entornos reales antes de declarar la versión como `stable` y congelar
  definitivamente su API y/o dar la oportunidad a la comunidad de probar features antes de su implementación.
- **Se asume**: Las versiones alpha pueden cambiar en cualquier momento su api, ser incompletas, ser solo
  característica incompleta, o incluso ser abortadas y eliminadas permanentemente sin previo aviso ni garantía,
  y por tanto, el usuario no las va a utilizar en producción.
