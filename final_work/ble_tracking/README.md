# Sistema de monitoramento e rastreamento de ativos

<div align="center">
    <img src="imgs/overview.png" 
         width="90%" 
         style="padding: 10px">
</div>


# Pacotes de Trabalho

> ## Rede de sensores - Beacon BLE

Este pacote de trabalho envolve o desenvolvimento da rede de sensores responsáveis por coletar os dados em campo. É divido em dois tipos de dispositivos:

### Beacons BLE
    
Desenvolvimento do firmware dos sensores BeaconBLE que serão fixados nas bobinas de chapa de aço, responsáveis por coletar a temperatura e transmitir através de um pacote beaconBLE. 

Pontos centrais de pequisa:

- Definição do protocolo de beacon a ser utilizado
- Informações transmitidas
- Requisitos não funcionais
    - Economia de energia

### Anchors BLE

Desenvolvimento do firware dos nodos fixos da rede (anchors) que irão receber os pacotes dos beacon, anotando o valor de sinal RSSI para posterior uso pelo algoritmo de localização das bobinas

Pontos centrais de pesquisa:

- Protocolos para transbordo dos dados dos beacons para o servidor central
- Sincronização dos dados recebidos dos beacons


> ## Servidor Central 

Este pacote de trabalho consiste na especificação e implementação dos serviços necessários no servidor central que irá processar os dados coletados da rede de sensores e prover a interface para visualização dos dados.

Pontos centrais de pesquisa:

- Protocolos e serviços para recebimento dos dados da rede de sensor sem fim
- Serviço para armazenamento dos dados recebidos de forma temporal no servidors
- Bibliotecas e suporte para os componentes utilizados pela aplicação de processamento e visualização de dados

> ## Aplicação de processamento de dados e visualização

Este pacote de trabalho consiste no desenvolvimento da aplicação para processamento dos dados coletados em campo (trilateração para localização dos sensores), assim como a visualização dos dados (temperatura e localização das bobinas).
 
Pontos centrais de pesquisa:

- Algoritmos de estimação da distância baseados em RSSI
- Algoritmos de trilateração baseado nas distâncias estimadas
- Bibliotecas e pacotes para viasualização de dados

