# Lab 04
<!--Empacotamento avançado-->

Objetivos:
  - Empacotar um aplicativo com uma dependência obrigatória e uma dependência opcional
  - Empacotar uma biblioteca, hospedada no GitHub
  - Utilizar *hooks* para ajustar pacotes
  - Adicionar um patch a um pacote

## Iniciando o empacotamento do aplicativo *bar*

Para fins deste treinamento, criamos um aplicativo completamente estúpido e inútil chamado *bar*. Sua página inicial é [https://bootlin.com/~thomas/bar/](https://bootlin.com/~thomas/bar/), onde você pode baixar um arquivo de origem do aplicativo.

Crie um pacote inicial para o *bar* no diretório `package/bar`, com o código necessário nos arquivos `package/bar/bar.mk` e `package/bar/Config.in`. Não esqueça do arquivo `package/bar/bar.hash`. Neste ponto, seu arquivo `bar.mk` deve apenas definir as variáveis `<pkg>_VERSION`, `<pkg>_SOURCE` e `<pkg>_SITE`, e fazer uma chamada para a infraestrutura de pacotes.

Habilite o pacote *bar* na configuração do Buildroot e inicie a compilação. Ele deve baixar o *bar*, extrair e iniciar o script de configuração. No entanto, ele deve falhar com um erro relacionado à biblioteca `libfoo`. De fato, como o arquivo `README` disponível no código-fonte do *bar* indica, ele tem uma dependência obrigatória na biblioteca `libfoo`. Portanto, vamos avançar para a próxima seção e iniciar o empacotamento da `libfoo`.

## Empacotando a `libfoo`: empacotamento inicial

De acordo com o arquivo `README` do *bar*, a biblioteca `libfoo` está disponível apenas no *GitHub* em [https://github.com/tpetazzoni/libfoo](https://github.com/tpetazzoni/libfoo).

Crie um pacote inicial para a `libfoo` no diretório `package/libfoo`, com as variáveis mínimas relevantes para baixar corretamente a `libfoo`. Como ela está hospedada no *GitHub*, lembre-se de usar a função `github` do *make* fornecida pelo Buildroot para definir a variável `<pkg>_SITE`. Para saber mais sobre essa função, pesquise por ela na árvore do Buildroot ou consulte o manual de referência do Buildroot.

Além disso, observe que há uma versão marcada como `v0.1` no repositório do *GitHub*, provavelmente você deve utilizá-la.

Habilite o pacote `libfoo` e inicie a compilação. Você deve obter um erro devido à ausência do script `configure`. O que pode ser feito sobre isso? Dica: existe uma variável no Buildroot para pacotes *autotools* que resolve esse problema.

Agora o `libfoo` deve ser compilado corretamente. Verifique o diretório `output/target/usr/lib`, a versão dinâmica da biblioteca deve estar instalada. No entanto, se você verificar o diretório `output/staging/`, não encontrará nenhum sinal da `libfoo`, nem a biblioteca em `output/staging/usr/lib` nem o arquivo de cabeçalho em `output/staging/usr/include`. Isso é um problema porque o compilador procurará bibliotecas e cabeçalhos apenas em `output/staging`, então precisamos modificar nosso pacote para que ele também seja instalado no diretório *staging*. Ajuste seu arquivo `libfoo.mk` para alcançar isso, reinicie a compilação do `libfoo` e verifique se você vê o arquivo `foo.h` em `output/staging/usr/include` e `libfoo.*` em `output/staging/usr/lib`.

Agora tudo parece estar correto, mas há algumas melhorias que podemos fazer.

## Melhorias no empacotamento da `libfoo`

Se você verificar o diretório `output/target/usr/bin`, encontrará um programa chamado `libfoo-example1`. Este é apenas um programa de exemplo para a `libfoo` e geralmente não é muito útil em um sistema de destino real. Portanto, gostaríamos que esse programa de exemplo não fosse instalado. Para conseguir isso, adicione um *hook* de destino *post-install* que remova o `libfoo-example1`. Reconstrua o pacote `libfoo` e verifique se o `libfoo-example1` foi removido corretamente.

Agora, se você for para o diretório `output/build/libfoo-v0.1` e executar `./configure --help` para ver as opções disponíveis, deverá ver uma opção chamada `--enable-debug-output`, que habilita um recurso de depuração da `libfoo`. Adicione uma subopção em `package/libfoo/Config.in` para habilitar o recurso de depuração e o código correspondente em `libfoo.mk` para passar `--enable-debug-output`. <!-- ou `--disable-debug-output` quando apropriado. -- Isso não funciona! HUgom-->

Habilite essa nova opção no `menuconfig` e reinicie a compilação do pacote. Verifique na saída da compilação se `--enable-debug-output` foi passado corretamente como argumento para o script `configure`.

Agora, o empacotamento da `libfoo` parece estar correto, então vamos voltar para nosso aplicativo *bar*.

## Finalize a embalagem do `bar`

Então, *bar* estava falhando na configuração porque estava faltando o `libfoo`. Agora que o `libfoo` está disponível, modifique *bar* para adicionar o `libfoo` como uma dependência. Lembre-se que isso precisa ser feito em dois lugares: no arquivo `Config.in` e no arquivo `bar.mk`.

Reinicie a compilação e ela deverá ser bem-sucedida! Agora você pode executar o aplicativo *bar* no seu destino e descobrir o quão absolutamente inútil ele é, exceto por permitir que você aprenda sobre a embalagem do Buildroot!

## Embalagem do `bar`: dependência *libconfig*

Mas há mais coisas que podemos fazer para melhorar a embalagem do *bar*. Se você for para `output/build/bar-1.0` e executar `./configure --help`, verá que ele suporta uma opção `--with-libconfig`. E de fato, o arquivo `README` do *bar* também menciona o `libconfig` como uma dependência opcional.

Portanto, altere o `bar.mk` para adicionar o *libconfig* como uma dependência opcional. Não é necessário adicionar uma nova opção `Config.in` para isso: apenas certifique-se de que quando o *libconfig* estiver habilitado na configuração do Buildroot, `--with-libconfig` seja passado para o script de configuração do *bar* e que o *libconfig* seja compilado antes do *bar*. Também passe `--without-libconfig` quando o *libconfig* não estiver habilitado.

Habilite o `libconfig` na configuração do Buildroot e reinicie a compilação do *bar*. O que acontece?

Falha na compilação com mensagens como `error: unknown type name ‘config_t’`. Parece que o autor do *bar* cometeu um erro e esqueceu de incluir o arquivo de cabeçalho apropriado. Vamos tentar corrigir isso: vá para o código-fonte do *bar* em `output/build/bar-1.0` e edite `src/main.c`. Logo após `#if defined(USE_LIBCONFIG)`, adicione `#include <libconfig.h>`. Salve e reinicie a compilação do *bar*. Agora ele compila corretamente!

No entanto, tente reconstruir o *bar* do zero usando `make bar-dirclean all`. O problema de compilação ocorre novamente. Isso ocorre porque fazer uma alteração diretamente em `output/build/` pode ser bom para fazer um teste rápido, mas não para uma solução permanente: tudo em `output/` é excluído ao executar `make clean`. Portanto, em vez de alterar manualmente o código-fonte do pacote, precisamos gerar um patch adequado para ele.

Existem várias maneiras de criar patches, mas vamos usar o Git para isso. Conforme indica a página inicial do *bar*, um repositório Git está disponível no GitHub em https://github.com/tpetazzoni/bar.

Comece clonando o repositório Git:

```
git clone https://github.com/tpetazzoni/bar.git
```


Após a clonagem, vá para o diretório do *bar* e crie um novo branch chamado `buildroot`, que parte da tag `v1.0` (que corresponde ao arquivo tar `bar-1.0.tar.xz` que estamos usando):

```
git branch buildroot v1.0
```

Mude para este novo branch:
\footnote{Sim, podemos usar `git checkout -b` para criar o branch e mudar para ele em um único comando

```
git checkout buildroot
```


Faça a alteração `#include <libconfig.h>` em `src/main.c` e faça o commit do resultado:

```
git commit -a -m "Fix missing <libconfig.h> include"
```


Gere o patch para o último commit (ou seja, o que você acabou de criar):

```
git format-patch HEAD^
```

e copie o arquivo `0001-*.patch` gerado para `package/bar/` nos arquivos do Buildroot.

Agora, reinicie a compilação com `make bar-dirclean all`, ela deverá ser concluída com sucesso!

Você pode até verificar que o *bar* está vinculado ao `libconfig.so` executando:

```
./output/host/usr/bin/arm-none-linux-gnueabihf-readelf -d output/target/usr/bin/bar
```


No destino, teste o *bar*. Em seguida, crie um arquivo chamado `bar.cfg` no diretório atual com o seguinte conteúdo:

```
verbose = "yes"
```

Execute o *bar* novamente e veja a diferença que ele faz.

Parabéns, você terminou de embalar o aplicativo mais inútil do mundo!

## Preparando-se para o próximo laboratório

Em preparação para o próximo laboratório, precisamos fazer uma nova compilação limpa, então basta executar:

```
make clean all 2>&1 | tee build.log
```

<!-- Markdown Reference

## header 1

### header 2

* italic *

**bold**

*** bold & italic ***

~~ strike-through ~~

> quote

` monospace-code `

```
code block
```
named footnote[^footnote01]
[^footnote01]: See https://elinux.org/images/e/ef/USB_Gadget_Configfs_API_0.pdf for more details

Repo link to file
[/labs/lab02/files/S30usbgadget](./files/S30usbgadget).

Image centered and scaled.
<p align="center"><img src="imgs/bbb-nunchuk-connection.jpg" alt="Nunchuk Connection" align="center" width="50%"/>

-->
