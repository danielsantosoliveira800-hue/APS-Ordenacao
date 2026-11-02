# Guia de instalação

Este guia explica, passo a passo, como instalar tudo o que o projeto precisa para compilar e executar. Depois de seguir os passos, volte ao [README](../README.md) para executar o programa.

## 1. O que é necessário

| Ferramenta | Para que serve |
|---|---|
| **Git** | Baixar (clonar) o repositório |
| **Compilador C (GCC)** | Compilar o programa em C |
| **CMake 3.20 ou superior** | Configurar a compilação |
| **Ninja** | Executar a compilação (usado pelos scripts) |
| **Python 3** | Gerar os gráficos e validar os resultados (usa pandas e Matplotlib) |

O CLion **não** é obrigatório. Escolha **uma** das opções abaixo para o seu sistema.

- [Windows, opção A: instalar tudo com o MSYS2 (recomendada)](#2-windows-opção-a-msys2-recomendada)
- [Windows, opção B: usar as ferramentas do CLion](#3-windows-opção-b-ferramentas-que-vêm-com-o-clion)
- [Linux (Ubuntu/Debian)](#4-linux-ubuntudebian)

---

## 2. Windows, opção A: MSYS2 (recomendada)

Nesta opção você instala apenas três programas: **Git**, **Python** e **MSYS2**. O MSYS2 fornece o GCC, o CMake e o Ninja de uma vez.

### Passo 1: instalar o Git

Baixe e instale em <https://git-scm.com/download/win>, mantendo as opções padrão. Ou, no PowerShell:

```powershell
winget install --id Git.Git -e
```

### Passo 2: instalar o Python

Baixe o instalador em <https://www.python.org/downloads/> (a versão testada neste projeto é a 3.13).

**Importante:** na primeira tela do instalador, marque a caixa **"Add python.exe to PATH"** antes de clicar em *Install Now*.

### Passo 3: instalar o MSYS2

Baixe e instale em <https://www.msys2.org/>, mantendo a pasta padrão `C:\msys64`. Ou, no PowerShell:

```powershell
winget install --id MSYS2.MSYS2 -e
```

### Passo 4: instalar GCC, CMake e Ninja pelo MSYS2

1. Abra o menu Iniciar e procure por **MSYS2 UCRT64** (é importante ser o *UCRT64*, e não os outros).
2. Na janela que abrir, atualize o sistema:

   ```bash
   pacman -Syu
   ```

   Se a janela fechar sozinha no final, abra o **MSYS2 UCRT64** de novo e repita o comando.
3. Instale as ferramentas:

   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
   ```

   Responda `Y` (ou pressione Enter) quando perguntar se deseja continuar.

### Passo 5: adicionar o MSYS2 ao PATH do Windows

Para que o PowerShell encontre o `gcc`, o `cmake` e o `ninja`, adicione a pasta `C:\msys64\ucrt64\bin` ao PATH. No PowerShell:

```powershell
[Environment]::SetEnvironmentVariable("Path", [Environment]::GetEnvironmentVariable("Path", "User") + ";C:\msys64\ucrt64\bin", "User")
```

Depois, **feche todas as janelas do PowerShell e abra uma nova**. A mudança só vale em janelas abertas depois deste comando. Rode o comando apenas uma vez.

Se preferir pela interface: pesquise por *"Editar as variáveis de ambiente do sistema"*, clique em *Variáveis de Ambiente*, selecione `Path` em *Variáveis de usuário*, clique em *Editar > Novo* e adicione `C:\msys64\ucrt64\bin`.

Siga para a [verificação](#5-verificar-a-instalação).

---

## 3. Windows, opção B: ferramentas que vêm com o CLion

Use esta opção se você já tem o CLion instalado e não quer instalar mais nada. O CLion traz o GCC e o CMake, mas o **Ninja que acompanha o CLion não funciona fora dele** (ele depende de uma biblioteca que só existe dentro do CLion). Por isso o Ninja é instalado pelo Python.

Você ainda precisa do **Git** e do **Python** (passos 1 e 2 da opção A).

No PowerShell, ajuste o número da versão do CLion se for diferente:

```powershell
$clion = "C:\Program Files\JetBrains\CLion 2026.2.2\bin"
$env:Path = "$clion\cmake\win\x64\bin;$clion\mingw\bin;" + $env:Path
python -m pip install ninja
$env:Path = (python -c "import sysconfig; print(sysconfig.get_path('scripts'))") + ";" + $env:Path
```

**Atenção:** essas mudanças valem só para a janela do PowerShell em que foram feitas. Ao abrir uma janela nova, é preciso repetir os comandos antes de rodar o `executar_windows.bat`. Para tornar permanente, adicione ao PATH do Windows as pastas `...\CLion <versão>\bin\cmake\win\x64\bin`, `...\CLion <versão>\bin\mingw\bin` e a pasta `Scripts` do Python (o comando `python -c "import sysconfig; print(sysconfig.get_path('scripts'))"` mostra o caminho).

---

## 4. Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build python3 python3-venv python3-pip git
```

Confirme que o CMake é da versão 3.20 ou superior (`cmake --version`). Se for mais antigo, instale uma versão mais nova pelo site <https://cmake.org/download/> ou com `pip install cmake`.

---

## 5. Verificar a instalação

Abra um **PowerShell novo** (ou o terminal, no Linux) e rode:

```powershell
git --version
python --version
cmake --version
gcc --version
ninja --version
```

Cada comando deve mostrar uma versão. Se algum disser que o comando não foi reconhecido, volte ao passo correspondente ou consulte a seção de problemas comuns abaixo.

---

## 6. Primeira execução

```powershell
git clone https://github.com/danielsantosoliveira800-hue/APS-Ordenacao.git
cd APS-Ordenacao
.\executar_windows.bat
```

No Linux:

```bash
git clone https://github.com/danielsantosoliveira800-hue/APS-Ordenacao.git
cd APS-Ordenacao
chmod +x executar_linux.sh
./executar_linux.sh
```

O script cria o ambiente Python, instala o pandas e o Matplotlib, compila o programa e abre o menu. Para apenas testar a instalação, digite `3` e pressione Enter para sair. Para rodar a coleta completa e gerar os gráficos de uma vez (cerca de 15 minutos), use o `executar_tudo.bat`, descrito no README.

---

## 7. Problemas comuns

| Mensagem ou sintoma | Causa provável | Solução |
|---|---|---|
| `CMake nao encontrado no PATH` (ou `gcc`, `ninja`, `python`) | A ferramenta não está instalada ou o PATH não foi atualizado | Refaça o passo do PATH e abra uma **nova** janela do PowerShell |
| `python` abre a Microsoft Store | O Windows usa um atalho que redireciona para a loja | Em *Configurações > Aplicativos > Aplicativos e recursos > Aliases de execução de aplicativo*, desative `python.exe` e `python3.exe` |
| `Exit code 0xc0000135` ao configurar o CMake | Está sendo usado o Ninja que vem com o CLion, fora do CLion | Use o Ninja do MSYS2 (opção A) ou instale com `python -m pip install ninja` (opção B) |
| `CMAKE_C_COMPILER not set` | O GCC não foi encontrado, ou houve falha anterior na configuração | Confira `gcc --version`, apague a pasta `build` e rode o script de novo |
| Erro de configuração depois de mudar de ferramenta | A pasta `build` guarda a configuração antiga | Apague a pasta `build` e execute novamente |
| `.\executar_windows.bat` não é reconhecido | Faltou o `.\` no PowerShell | Digite `.\executar_windows.bat` dentro da pasta do projeto |
| `windows.h: No such file` no Linux | Código de teste de uma versão antiga | Atualize o repositório com `git pull` |

---

## 8. Versões testadas

O projeto foi compilado e executado com:

- Windows 10 Pro, CMake 4.3.1, GCC 15.2.0 (MinGW), Ninja 1.13.2 e Python 3.13.7.
- Linux, em ambiente de teste, com GCC e CMake instalados pelo gerenciador de pacotes.

Versões próximas devem funcionar, desde que o CMake seja 3.20 ou superior.
