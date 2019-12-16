# driverPCIealveoNUplus

Per poter caricare ed avviare un kernel su Nuplus bisogna utilizzare degli script forniti da Xilinx che si trovano a questo indirizzo

Prima di tutto devi caricarti il driver per poter utilizzare il PCIE che si trova nella cartella tests lo script è  XDMA/linux-kernel/tests/load_driver.sh, poi compilati il Kernel ed il file “nome_del_programma”_mem_standalone.hex, per caricarlo in memoria puoi utilizzare questo script presente nella cartella XDMA/linux-kernel/tools
```shell
./dma_to_device -d /dev/xdma0_h2c_0 -f /home/smilo/Desktop/npu_test/obj/prova_mem_standalone.hex -s 18612508 -a 0x0000000020000000  -v -w ../tests/data/provamakemake
```
dove -d indica il il dispositivo -f il file da caricare -s il numero di byte (il puoi vedere usando il comando wc -c sul file) -a l’ indirizzo -v è la verbose mode -w ti scrive un file per farti capire cosa il tool ha scritto

Dopodiché per poter lanciare il kernel io per il momento resetto nuplus con questo comando 
```shell
sudo ./dma_to_device_binary -d /dev/xdma0_h2c_0 -f ../tests/data/mio.bin -s 8 -a 0x0000000000000000  -v -w ../tests/data/prova
```
dove il file mio.bin ha la configurazione per resettare nuplus, il router per gestire le comunicazioni ed il memory controller.
Infine avvio il kernel con questa utility:
``` shell
sudo ./polling
```
Questo programmino creato da me nel main ha una serie di comandi che puoi decidere di inviare oppure no, per poter avviare il kernel servono i primi tre, uno che disattiva i thread se sono attivi, un altro che setta il PC ed infine l’ ultimo che avvia i thread, gli altri comandi sono solo di controllo.

Se ti serve recuperare la memoria dopo un’esecuzione puoi utilizzare:
``` shell
 ./dma_from_device -d /dev/xdma0_c2h_0 -f ../tests/data/prova -s 0x01000000 -a 0x20000000 -c 1
```
Se ti serve vedere l’ indirizzi del HBM li puoi trovare nella sezione Address Editor del block design.
