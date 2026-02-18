#!/usr/bin/env bash

# --- Configuración de Colores ---
BOLD="\033[1m"
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
CYAN="\033[36m"
RESET="\033[0m"

# --- Resolución de Rutas ---
# Source - https://stackoverflow.com/a/246128
# Posted by dogbane, modified by community. See post 'Timeline' for change history
# Retrieved 2026-02-16, License - CC BY-SA 4.0
get_script_dir() {
    local SOURCE="${BASH_SOURCE[0]}"
    while [ -L "$SOURCE" ]; do
        local DIR
        DIR="$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)"
        SOURCE="$(readlink "$SOURCE")"
        [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
    done
    local DIR
    DIR="$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)"
    echo "$DIR"
}

SCRIPT_DIR="$(get_script_dir)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Ruta real a los includes
INCLUDE_ROOT="$PROJECT_ROOT/saburou_platform/include/saburou/platform"

# --- Gestión de Directorio Temporal (Seguridad Máxima) ---
# Creamos un directorio temporal seguro en /tmp (o donde diga el sistema)
STAGING_DIR=$(mktemp -d -t saburou-refactor.XXXXXX)

# TRAP: Pase lo que pase (éxito o error), al salir del script se borra el temp
trap 'rm -rf "$STAGING_DIR"' EXIT

usage() {
    echo -e "${BOLD}Uso:${RESET} $0 -c <version_actual> -n <nueva_version>"
    echo "  -c : Versión origen (ej: v1)"
    echo "  -n : Versión destino (ej: v2-alpha)"
    exit 1
}

# --- Parseo de Argumentos ---
while getopts "c:n:" opt; do
    case $opt in
    c) CUR_VER="$OPTARG" ;;
    n) NEW_VER="$OPTARG" ;;
    *) usage ;;
    esac
done
shift $((OPTIND - 1))

# --- Validaciones de Entrada ---
if [[ -z "$CUR_VER" || -z "$NEW_VER" ]]; then usage; fi
if [[ ! "$CUR_VER" =~ ^v[0-9] || ! "$NEW_VER" =~ ^v[0-9] ]]; then
    echo -e "${RED}❌ Error:${RESET} Las versiones deben empezar por 'v' (ej: v1, v2-alpha)."
    exit 1
fi

# --- Validación de Archivos ORIGEN ---
SOURCE_HEADER="$INCLUDE_ROOT/$CUR_VER.hpp"
SOURCE_DIR="$INCLUDE_ROOT/$CUR_VER"

if [[ ! -f "$SOURCE_HEADER" ]]; then
    echo -e "${RED}❌ Error (Origen):${RESET} No existe el header: $SOURCE_HEADER"
    exit 1
fi
if [[ ! -d "$SOURCE_DIR" ]]; then
    echo -e "${RED}❌ Error (Origen):${RESET} No existe el directorio: $SOURCE_DIR"
    exit 1
fi

# --- Validación de Archivos DESTINO (Safety check) ---
DEST_HEADER="$INCLUDE_ROOT/$NEW_VER.hpp"
DEST_DIR="$INCLUDE_ROOT/$NEW_VER"

if [[ -f "$DEST_HEADER" ]] || [[ -d "$DEST_DIR" ]]; then
    echo -e "${RED}❌ Error (Destino):${RESET} La versión destino YA EXISTE."
    echo -e "   Conflicto en: $INCLUDE_ROOT/$NEW_VER..."
    exit 1
fi

# --- Procesamiento de Strings ---
process_version() {
    local ver="$1"
    local type="$2"
    case "$type" in
        macro|ns)
            local safe_ver="${ver//-/_}"
            if [[ "$type" == "macro" ]]; then echo "${safe_ver^^}"; else echo "${safe_ver,,}"; fi
            ;;
        import)
            local safe_ver="${ver//_/-}"
            echo "${safe_ver,,}"
            ;;
    esac
}

OLD_MACRO="SABUROU_PLATFORM_$(process_version "$CUR_VER" "macro")"
NEW_MACRO="SABUROU_PLATFORM_$(process_version "$NEW_VER" "macro")"
OLD_NS="saburou::platform::$(process_version "$CUR_VER" "ns")"
NEW_NS="saburou::platform::$(process_version "$NEW_VER" "ns")"
OLD_IMPORT="saburou/platform/$(process_version "$CUR_VER" "import")"
NEW_IMPORT="saburou/platform/$(process_version "$NEW_VER" "import")"

# --- Resumen Visual (El formato que pediste) ---
echo -e "${CYAN}${BOLD}🔧 PLAN DE REFACTORIZACIÓN${RESET}"
echo -e "------------------------------------------------"
echo -e "📂 Working Dir:   (Temporal) $STAGING_DIR"
echo -e "🚀 Destino Final: $INCLUDE_ROOT/"
echo -e "------------------------------------------------"
echo -e "🔄 Macro:      ${RED}$OLD_MACRO${RESET} -> ${GREEN}$NEW_MACRO${RESET}"
echo -e "🔄 Namespace:  ${RED}$OLD_NS${RESET} -> ${GREEN}$NEW_NS${RESET}"
echo -e "🔄 Path:       ${RED}$OLD_IMPORT${RESET} -> ${GREEN}$NEW_IMPORT${RESET}"
echo -e "📄 Header:     ${RED}$CUR_VER.hpp${RESET} -> ${GREEN}$NEW_VER.hpp${RESET}"
echo -e "📂 Carpeta:    ${RED}$CUR_VER/${RESET}    -> ${GREEN}$NEW_VER/${RESET}"
echo -e "------------------------------------------------"
echo -n -e "Confirmar y aplicar cambios? [y/N]: "
read -r response

if [[ ! "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo -e "${YELLOW}Abortado.${RESET}"
    exit 0 # El trap borrará STAGING_DIR automáticamente
fi

# --- FASE 1: Clonado al Temp ---
echo -e "\n📦 Copiando al área de trabajo..."
cp "$SOURCE_HEADER" "$STAGING_DIR/"
cp -r "$SOURCE_DIR" "$STAGING_DIR/"

# --- FASE 2: Refactorización (Perl) ---
echo -e "📝 Aplicando regex..."

# Macros
find "$STAGING_DIR" -type f \( -name "*.hpp" -o -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 \
    perl -pi -e "s/${OLD_MACRO}/${NEW_MACRO}/g"

# Namespace
find "$STAGING_DIR" -type f \( -name "*.hpp" -o -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 \
    perl -pi -e "s/${OLD_NS}/${NEW_NS}/g"

# Imports
find "$STAGING_DIR" -type f \( -name "*.hpp" -o -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 \
    perl -pi -e "s|([\"<])\Q${OLD_IMPORT}\E|\1${NEW_IMPORT}|g"

# --- FASE 3: Renombrado Físico en Temp ---
mv "$STAGING_DIR/$CUR_VER.hpp" "$STAGING_DIR/$NEW_VER.hpp"
mv "$STAGING_DIR/$CUR_VER" "$STAGING_DIR/$NEW_VER"

# --- FASE 4: Despliegue Final ---
echo -e "🚀 Moviendo archivos al directorio include..."
mv "$STAGING_DIR/$NEW_VER.hpp" "$INCLUDE_ROOT/"
mv "$STAGING_DIR/$NEW_VER" "$INCLUDE_ROOT/"

echo -e "${GREEN}${BOLD}✅ Refactorización completada.${RESET}"
# Al salir aquí, el 'trap' limpiará STAGING_DIR
