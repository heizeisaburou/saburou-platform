# saburou-platform

**saburou-platform** es una librería moderna de C++23 diseñada para la introspección profunda del entorno de
ejecución. Su núcleo combina un motor de detección basado en preprocesador de alta precisión con una interfaz
de usuario limpia, fuertemente tipada y lista para `std::format`.

- **Estándar Requerido:** C++23.
- **Licencia:** Apache 2.0 (ver archivo `LICENSE.md`).

---

## 🛠️ Instalación y Uso

### Opción A: Como dependencia (GitHub / FetchContent)

Puedes integrar la librería directamente en tu `CMakeLists.txt` sin instalar nada en el sistema:

```cmake
include(FetchContent)
FetchContent_Declare(
    saburou-platform
    GIT_REPOSITORY [https://github.com/tu-usuario/saburou-platform.git](https://github.com/tu-usuario/saburou-platform.git)
    GIT_TAG main
)
FetchContent_MakeAvailable(saburou-platform)

target_link_libraries(tu_proyecto PRIVATE saburou::platform)
```

### Opción B: Instalación en el Sistema (Linux/macOS/Windows)

Para instalar la librería en `/usr/local/include` (o el equivalente en tu OS) y que esté disponible
globalmente:

```bash
# Configurar y compilar
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Instalar (requiere sudo en Linux/macOS)
sudo cmake --install build
```

**¿Cómo desinstalar?** CMake registra cada archivo instalado. Para limpiar el sistema, ejecuta desde la carpeta
del proyecto:

```bash
cat build/install_manifest.txt | xargs sudo rm
```

---

## 🛠 Motor de Detección (Macros)

La librería analiza el entorno mediante directivas de preprocesador, ofreciendo macros normalizadas para evitar
las inconsistencias entre compiladores.

### 1. Compiladores y Versiones

- `SABUROU_PLATFORM_V1_NVCC`: NVIDIA CUDA.
- `SABUROU_PLATFORM_V1_INTEL_CLASSIC`: Intel ICC/ICPC.
- `SABUROU_PLATFORM_V1_INTEL_LLVM`: Intel ICX/ICPX.
- `SABUROU_PLATFORM_V1_CLANG`: LLVM Clang.
- `SABUROU_PLATFORM_V1_BORLAND`: Embarcadero/Borland.
- `SABUROU_PLATFORM_V1_GCC`: GNU Compiler Collection.
- `SABUROU_PLATFORM_V1_MSVC`: Microsoft Visual C++.

### 2. Librería Estándar (C++ STDLIB)

- `SABUROU_PLATFORM_V1_STDLIB_LIBCXX`: LLVM libc++.
- `SABUROU_PLATFORM_V1_STDLIB_LIBSTDCXX`: GNU libstdc++.
- `SABUROU_PLATFORM_V1_STDLIB_MSVC`: Microsoft STL.

### 3. Sistemas Operativos y Plataformas

- **Windows:** `SABUROU_PLATFORM_V1_OS_WINDOWS`.
- **Apple:** `SABUROU_PLATFORM_V1_OS_DARWIN`, `SABUROU_PLATFORM_V1_OS_MACOS`, `SABUROU_PLATFORM_V1_OS_IOS`,
  `SABUROU_PLATFORM_V1_OS_IOS_SIMULATOR`.
- **BSD:** `SABUROU_PLATFORM_V1_OS_BSD` (FreeBSD, NetBSD, OpenBSD, DragonFly).
- **Linux/Android:** `SABUROU_PLATFORM_V1_OS_LINUX`, `SABUROU_PLATFORM_V1_OS_ANDROID`.
- **WASM:** `SABUROU_PLATFORM_V1_OS_WASM`, `SABUROU_PLATFORM_V1_OS_WASM_EMSCRIPTEN`,
  `SABUROU_PLATFORM_V1_OS_WASM_WASI`.
- **POSIX:** `SABUROU_PLATFORM_V1_POSIX_LIKE`.

### 4. Arquitectura y Hardware

- **ISA:** `SABUROU_PLATFORM_V1_ARCH_X86` (32/64), `SABUROU_PLATFORM_V1_ARCH_ARM` (32/64),
  `SABUROU_PLATFORM_V1_ARCH_RISCV` (32/64), `SABUROU_PLATFORM_V1_ARCH_PPC` (32/64).
- **Bitness:** `SABUROU_PLATFORM_V1_BITNESS_64`, `SABUROU_PLATFORM_V1_BITNESS_32`,
  `SABUROU_PLATFORM_V1_BITNESS_16`.
- **Endianness:** `SABUROU_PLATFORM_V1_LITTLE_ENDIAN`, `SABUROU_PLATFORM_V1_BIG_ENDIAN`.
- **Caché:** `SABUROU_PLATFORM_V1_CACHELINE` (usa `hardware_destructive_interference_size` si está disponible).

### 5. ABI y Entorno

- **Data Model:** `SABUROU_PLATFORM_V1_ABI_LP64`, `SABUROU_PLATFORM_V1_ABI_LLP64`,
  `SABUROU_PLATFORM_V1_ABI_ILP32`.
- **Environment:** `SABUROU_PLATFORM_V1_ENV_HOSTED`, `SABUROU_PLATFORM_V1_ENV_FREESTANDING`.

---

## 🧬 Interfaz de Programación (API)

Toda la API reside bajo el namespace `saburou::platform::v1`.

### Identificación del OS (Compile-time)

Se utilizan funciones `consteval` para obtener la información sin coste en runtime.

- `os::family()`: Retorna un `family_t` (windows, darwin, bsd, android, wasm, linux, posix, unknown).
- `os::type()`: Retorna un `type_t` más específico (macos, ios, ios_simulator, freebsd, emscripten, etc.).
- `os::family_name()`: Retorna el nombre legible de la familia como `const char*`.
- `os::name()`: Retorna el nombre legible del tipo de OS como `const char*`.

**Funciones Auxiliares:**

- `os::to_code_name(family_t/type_t)`: Identificador técnico en minúsculas (ej: "linux").
- `os::to_family_name(family_t)`: Nombre formal (ej: "WebAssembly").
- `os::to_name(type_t)`: Nombre formal específico (ej: "iOS Simulator").

### Información del Sistema (Runtime)

Para datos que no pueden conocerse hasta que el programa corre:

- `os::info()`: Retorna una estructura `info_t` con la versión del kernel (`version_t`) y el string de release
  original. En sistemas POSIX usa `uname`.
- `os::linux::distro_info()`: (Solo Linux) Parsea `/etc/os-release` para obtener el ID, NAME, VERSION y
  BUILD_ID de la distribución actual.

---

## 📊 Estructuras de Datos

### version_t

Estructura semántica para versiones:

- `major`, `minor`, `patch` (int).

### info_t

Agregado de información del sistema:

- `version_t version`.
- `std::string release_str`.

### distro_info_t (Linux)

Metadatos de la distribución:

- `id` (ej: "arch").
- `name` (ej: "Arch Linux").
- `version`, `build_id`.

---

## 🖋 Formateo (std::formatter)

Todos los tipos de la librería soportan `std::format` con los siguientes especificadores:

- `{}` o `{:s}`: Salida estándar/amigable.
- `{:r}`: Representación técnica detallada (Qualified representation).

```cpp
#include <saburou/platform/v1.hpp>
#include <saburou/platform/v1/os/linux.hpp> // distro_info

#include <format>
#include <iostream>

int main() {

    auto type = saburou::platform::v1::os::type();
    std::cout << std::format("type\n  {:r} -> {}\n", type, type);  // r == repr (python based)
    auto name = saburou::platform::v1::os::to_name(type);
    std::cout << std::format("(type_)name\n  {}\n", name);

    std::cout << "\n";
    auto family = saburou::platform::v1::os::family();
    std::cout << std::format("family\n  {:r} -> {}\n", family, family);
    auto family_name = saburou::platform::v1::os::to_family_name(family);
    std::cout << std::format("family_name\n  {}\n", family_name);

    std::cout << "\n";
    auto info = saburou::platform::v1::os::info();
    std::cout << "(os_)info\n";
    std::cout << std::format("  [repr]  {:r}\n", info);
    std::cout << std::format("[normal]  {:s}\n", info);

    std::cout << "\n";
    auto distro_info = saburou::platform::v1::os::linux::distro_info();
    std::cout << "distro_info\n";
    std::cout << std::format("  [repr]  {:r}\n", distro_info);
    std::cout << std::format("[normal]  {}\n", distro_info); // same as :s
}
```

---

## ⚠️ Utilidades y Mantenimiento

En la carpeta `utils/` se encuentra el script `refactor_platform.sh`.

> [!CAUTION] **Aviso de Seguridad:** Este script automatiza el renombrado de versiones, namespaces y macros en
> todo el proyecto. Ha sido diseñado para uso interno, está en fase experimental y **no debe ser utilizado** a
> menos que sepas exactamente lo que haces, ya que modifica archivos físicos de forma masiva.
