Branche DevRTC_StdBy_LL

-> Le développement HAL standby et RTC est un échec, le processeur est bricked car endormi à jamais...
nRST et VCC sont en CC sérieux. Voir de plus près.
-> Jusque là le code est vite lourd, 40% facile sans avoir trop rien fait...

Dans cette branche, on essaie de voir la conception en LL pour diminuer le code

Le répertoire de travail est /ExploreCodeStdby/MyNucleoL476_StdByPower

On crée un répertoire à partir de cube, en conception LL.
On va chercher l'exemple LL :
C:\Users\trocache\STM32Cube\Repository\STM32Cube_FW_L4_V1.17.2\Projects\NUCLEO-L476RG\Examples_LL\PWR\PWR_EnterStandbyMode

On récupère les 3 src et les 3 inc, que l'on copie dans le projet juste créé (remplacement de ceux créés par cube).
Compilation OK
SRC :
main.c 
stm32l4xx_it.c
system_stm32l4xx.c (est le même)

INC
main.h
stm32_assert.h
stm32l4xx_it.h



OBJ 1 : on fait idem mais périodique avec le wakeup timer.
OBJ 2 : faire tourner l'I2C en LL.


****************************
50abc65 
OBJ 1 en cours, bac à sable ajouté pour récupérer des codes générés automatiquement Cube.
/MyNucleoL476_StdByPower modifié, inspiré de RTC_ExitStandbyWithWakeUpTimer
A faire : Bien lire et faire une synthèse sur wakeup RTC Standby

****************************
349ff09
/ExploreCodeStdby / BacASable_LLFromCube
				  / MyNucleoL476_StdByPower
Programme opérationnel en standby mode !
		* PWR reset : LED s'allume 5s
		* nRST : LED s'allume 2s
		* standby wake up : LED s'allume 200ms


****************************
35642cd		
/ExploreCodeStdby / Remote_L041_StdByPower

Même programme sur 32L041F6P7
Programme opérationnel en standby mode !
		* PWR reset : PA1 (LEDIR) on 6s
		* nRST : PA1 (LEDIR) on  3s
		* standby wake up : PA1 (LEDIR) on  1s
		
Le tout sur le proto n°1 fait pas José.


****************************
35642cd   commit suivant
/ExploreCodeStdby / Remote_L041_StdByPower

Le programme fais une émission en boucle UART via FSK. Fonctionne à 38400 Bds, test en réception avec la carte tecom 103 de 4IS.
OK.
A faire réception.



	