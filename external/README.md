# Buildroot External

Para utilizar, crie a configuração do buildroot setando a variável de ambiente BR_EXTERNAL

make -C <<buildroot_path>> BR2_EXTERNAL=../external O=<<output_dir>> menuconfig
