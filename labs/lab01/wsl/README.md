# Configuração do Windows Subsystem for Linux (WSL) para uso do Buildroot

## Instalando o WSL no Windows

Inicialmente o WSL deve ser instalado, para isso, abra um terminal PowerShell em modo Administrador e execute o comando:

```
wsl --install -d Ubuntu
```

O processo de instalação pode demorar um pouco devido ao download de todos os componentes necessários, caso não estejam previamente instalados. Ao final será solicitado um nome de usuário e senha. Pode escolher livremente esses dados, mas use uma senha que não irá se esquecer, pois precisaremos dela sempre que executarmos comandos com o sudo.

Maiores detalhes sobre a instalação do WSL podem ser consultados no site https://learn.microsoft.com/pt-br/windows/wsl/install.

## Evitando a integração o WSL com o caminho de executáveis do Windows

O WSL no Windows possui uma integração que permite a execução de comandos nativos do Windows, dentro do ambiente do WSL. Apesar se ser uma integração interessante, a mesma gera um conflito para o uso do Buildroot, relacionado a variável de ambiente $PATH. Por este motivo, iremos desativar essa integração.

Para isso, iremos editar dentro do Ubuntu instalado no WSL o arquivo /etc/wsl.conf. Crie o arquivo, caso o mesmo não exista. Dentro deste arquivo, garanta que as seguintes configurações estejam ativas:

```
[interop]
enabled = false
appendWindowsPath = false
```

## Conectando dispositivos USB ao WSL

Para permitir a conexão de dispositivos USB diretamente ao WSL, é necessário a instalação do USBIPD, que cria um serviço para roteamente dos dados USB via IP para a máquina virtual do WSL.

Para instalar este componente, siga as instruções do site https://docs.microsoft.com/pt-br/windows/wsl/connect-usb

```
winget install --interactive --exact dorssel.usbipd-win
```
Instalar as ferramentas USBIP e o banco de dados de hardware no Linux

Depois que o projeto USB/IP tiver concluído a instalação, você precisará instalar as ferramentas de espaço do usuário e um banco de dados de identificadores de hardware USB. Essas instruções são para o Ubuntu – outras distribuições podem exigir um pacote cliente usbip diferente.

No Ubuntu 20.04, execute este comando:
```
sudo apt install linux-tools-5.4.0-77-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/5.4.0-77-generic/usbip 20
```
No Ubuntu 22.04, execute este comando:
```
sudo apt install linux-tools-5.15.0-67-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/5.15.0-67-generic/usbip 20
```

Após instalado, utilizaremos os seguintes comandos para conectar um dispositivo USB no Ubuntu do WSL.

Em uma interface do PowerShell, utilize o comando abaixo para listar todos os dispositivos que estão conectados na USB do computador. 

```
usbipd wsl list
```
Uma vez identificado o dispositivo que deseja conectar ao Ubuntu no WSL, utilize o comando abaixo, substituindo o <busid> pelo identificador listado pelo comando anterior.

```
usbipd wsl attach --busid <busid>
```

Caso esteja conectando o dispositivo pela primeira vez, é necessário executar o comando attach do usbipd em um terminal do PowerShell privilegiado (modo Administrador). Após a primeira conexão (do mesmo dispositivo), não é mais necessário o uso do terminal privilegiado.

Caso queira desconectar o dispositivo USB do WSL, utilize o comando:

```
usbipd wsl detach --busid <busid>
```

## Atualizando o kernel do WSL para suportar o Leitor de cartão SD e conversor serial

O WSL utiliza uma versão de kernel no Ubuntu customizada para integrar-se melhor com o Windows, desta forma, para que o leitor de cartão SD e o conversor USB-Serial que utilizamos seja reconhecido no Ubuntu, é necessário recompilar o kernel para o WSL, com o suporte para esses dispositivos.

Para isso iremos utilizar o próprio WSL para gerar a imagem do kernel utilizado pelo WSL, baseado nas instruções presentes no link https://microhobby.com.br/blog/2019/09/21/compiling-your-own-linux-kernel-for-windows-wsl2/

### Passo a passo resumido

1. Instalar no WSL as ferramentas para compilação do kernel:

```
sudo apt install git bc build-essential flex bison libssl-dev libelf-dev dwarves
```

2. Baixar o código fonte do kernel modificado pelo Microsoft e entrar na pasta:

```
git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
cd WSL2-Linux-Kernel
```

3. Habilitar os drivers necessários para configuração do kernel

   1. Abrir a ferramenta KCONFIG com o comando

   ```
   make KCONFIG_CONFIG=Microsoft/config-wsl menuconfig
   ```

   2. Habilitar a configuração "USB Mass Storage support" (CONFIG_USB_STORAGE=y) no menu:
      - Device Drivers
        - USB Support
          - USB Mass Storage support

      O item deve ser marcado com o "*" e não como módulo (m).
   
   3. Habilitar a configuração "USB Prolific 2303 Single Port Serial Driver" (CONFIG_USB_SERIAL_PL2303=y) no menu:
      - Device Drivers
        - USB Support
          - USB Serial Converter support
            - USB Prolific 2303 Single Port Serial Driver

      O item deve ser marcado com o "*" e não como módulo (m).

   4. Sair da ferramenta, confirmando que a configuração deve ser salva ao final.

   5. Dentro da árvore do kernel, edite manualmente o arquivo Microsoft/config-wsl, alterando o texto da configuração CONFIG_LOCALVERSION, para identificar a versão do kernel que está utilizando. A sugestão é para utilizar o texto "-microsoft-IFSC-lxe22109-WSL2"
   
4. Iniciar o processo de compilação do kernel através do comando:

   ```
   make KCONFIG_CONFIG=Microsoft/config-wsl -j8
   ```

5. Ao final do processo de compilação, será gerado um arquivo chamado vmlinux. Este arquivo deve ser copiado para o diretório HOME do seu usuário no Windows (Ex. C:\Users\hugom\vmlinux)

6. Criar um arquivo no seu diretório HOME (C:\Users\hugom) chamado .wslconfig com o seguinte conteúdo:

```
[wsl2]
kernel=C:\\Users\\hugom\\vmlinux
```
   
   Observe o uso de contrabarra duplas, e não esquece de substituir o nome da pasta (hugom) pelo nome da pasta do seu usuário no Windows.

7. Neste momento o WSL no seu usuário do Windows já está configurado para utilizar o kernel que compilamos com as modificações necessárias. Para certificarmos que o mesmo está funcional, proceda com os seguintes passos:

   1. Desligue a instância do WSL executando o seguinte comando:

   ```
   wsl --shutdown
   ```

   2. Inicie o WSL novamente, podendo abrir Ubuntu no Windows, ou simplesmente executando o comando wsl.exe no PowerShell

   3. Dentro do Ubuntu, execute o comando abaixo:

   ```
   uname -a
   ```

   4. A saída deste programa deve conter o mesmo texto que foi configurado na config CONFIG_LOCALVERSION, na etapa de compilação do kernel para o WSL. Veja o exemplo de saída com a sugestão mencionada:

   `Linux stardust 5.15.74.2-microsoft-IFSC-lxe22109-WSL2+ #1 SMP Wed Nov 16 14:42:42 -03 2022 x86_64 x86_64 x86_64 GNU/Linux`

## Conclusão

Com a realização destes passos, a sua instalação do WSL no Windows está apta a seguir os passos dos laboratórios que iremos executar. Haverá algumas diferenças para alguns comandos quando for necessário realizar o acesso à dispositivos USB. A documentação dos laboratórios irá detalhar essas diferenças quando necessário. 

Clique no link abaixo para continuar na preparação do seu ambiente de desenvolvimento para uso do Buildroot

[Lab01 - Tutorial](../README.md)
