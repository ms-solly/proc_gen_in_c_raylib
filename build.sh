#!/bin/bash

# Configuration (can be customized as needed)
PROJECT_NAME="${PROJECT_NAME:-craylib}"
RAYLIB_PATH="${RAYLIB_PATH:-../..}"
SRC_DIR="${SRC_DIR:-src}"
OBJ_DIR="${OBJ_DIR:-obj}"
BUILD_MODE="${BUILD_MODE:-RELEASE}"

# Compiler and flags
CC="gcc"
CFLAGS="-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces"
LDFLAGS="-L. -L${RAYLIB_PATH}/src"
INCLUDE_PATHS="-I. -I${RAYLIB_PATH}/src -I${RAYLIB_PATH}/src/external"
LDLIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

# Add debug or release flags
if [[ "$BUILD_MODE" == "DEBUG" ]]; then
    CFLAGS+=" -g -O0"
else
    CFLAGS+=" -s -O1"
fi

# Create object directory if it doesn't exist
mkdir -p "$OBJ_DIR"

# Collect sources
SRC_FILES=("$SRC_DIR"/*.c)
OBJ_FILES=()

# Compile sources
for SRC in "${SRC_FILES[@]}"; do
    OBJ="$OBJ_DIR/$(basename "${SRC%.c}.o")"
    echo "Compiling $SRC -> $OBJ"
    $CC $CFLAGS $INCLUDE_PATHS -c "$SRC" -o "$OBJ"
    OBJ_FILES+=("$OBJ")
done

# Link objects
echo "Linking ${OBJ_FILES[*]} -> $PROJECT_NAME"
$CC $LDFLAGS -o "$PROJECT_NAME" "${OBJ_FILES[@]}" $LDLIBS

echo "Build finished: $PROJECT_NAME"


./craylib
