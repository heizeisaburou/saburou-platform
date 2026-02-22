# Changelog - saburou-platform v2

## [0.2.0-beta] - Thu 2026-02-19

### Added

- **Sanitization Engine**: Sección 10 en `detect.hpp` que garantiza que todas las macros no detectadas se
  definan como `0`, permitiendo uso seguro de `#if` y compatibilidad con `-Wundef`.
- **Byte Manipulation**: Concepto `ByteSwappable` y utilidades de conversión `to_little/big` y
  `from_little/big` optimizadas con `if constexpr`.
- **C++20/23 Integration**: Uso de `std::endian` para validación estática de heurísticas y `std::byteswap` para
  operaciones de bajo nivel.

### Fixed

- **Macro Safety**: Eliminación de colisiones de nombres y avisos de redefinición mediante `#undef` de macros
  auxiliares internas.
- **Validation**: Implementación de `static_assert` para asegurar que las macros del preprocesador coincidan
  con la realidad del compilador.

### Changed

- **Zero-Cost Abstraction**: Migración de lógica de preprocesador a constantes `constexpr` para mejorar la
  legibilidad sin sacrificar rendimiento.
