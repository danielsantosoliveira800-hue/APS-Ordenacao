cmake_minimum_required(VERSION 3.20)

project(APS_Ordenacao LANGUAGES C)

# Define o padrão C11 e exige sua utilização.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Desativa extensões específicas do compilador.
set(CMAKE_C_EXTENSIONS OFF)

# Localiza os arquivos-fonte em C.
file(GLOB SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Cria o executável principal.
add_executable(APS_Ordenacao ${SOURCES})

# Mantém o executável na raiz do diretório de compilação.
set_target_properties(APS_Ordenacao PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Ativa avisos úteis durante a compilação.
if(MSVC)
    target_compile_options(APS_Ordenacao PRIVATE /W4)
else()
    target_compile_options(APS_Ordenacao PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

# Habilita as declarações POSIX necessárias para clock_gettime no Linux.
if(UNIX)
    target_compile_definitions(APS_Ordenacao PRIVATE _POSIX_C_SOURCE=200809L)
endif()
