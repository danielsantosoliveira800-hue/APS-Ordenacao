cmake_minimum_required(VERSION 3.20)

project(APS_Ordenacao LANGUAGES C)

# Usa Release por padrão (otimização -O3), para que os tempos medidos sejam comparáveis
# com os resultados de referência. O CLion e a linha de comando podem sobrescrever esse valor.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de build" FORCE)
endif()

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
    # Biblioteca matemática (sqrt, usada no cálculo do desvio-padrão).
    target_link_libraries(APS_Ordenacao PRIVATE m)
endif()

# Testes automatizados dos seis algoritmos de ordenacao.
include(CTest)
if(BUILD_TESTING)
    add_executable(test_ordenacao
            tests/test_ordenacao.c
            src/ordenacao.c
    )
    target_include_directories(test_ordenacao PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

    if(MSVC)
        target_compile_options(test_ordenacao PRIVATE /W4)
    else()
        target_compile_options(test_ordenacao PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME algoritmos_ordenacao COMMAND test_ordenacao)
endif()
