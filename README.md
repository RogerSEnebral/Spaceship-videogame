# Spaceship-videogame
 A classical spaceship videogame implemented in C++ with OpenGL.

# Execució
Per executar, entra a Binari i clica "Navesita.exe".

# Instruccions

Et mous per el menú amb les fletxes o amb "W" i "S". Prem enter per seleccionar una opció, i enter de nou per tornar.

Al jugar et mous amb "wasd" o amb les fletxes. Per disparar la nau s'utilitza el clic del ratolí. Clics ràpids pels tirs bàsics, i mantenir i soltar el botó del ratolí pel tir carregat.

Tocar enemics o bales d'enemics et mata inmediatament. El dispositiu force és invulnerable i el pots utilitzar com escut per parar bales.

Pulsa "f" per millorar en un nivell el force fins al 7, i "g" per tornarte invulnerable. Aquest mode et permetra passar a través de bales i enemics.
No obstant, dona igual si tens mode invulnerable o no, Si et quedes atascat entre l'inici de la pantalla i un bloc davant teu de tal manera que no pots avançar, moriràs aplastat.

Pulsa esc en el joc per sortir al menú principal, i en el menú per tancar el joc.

Esquiva, dispara i encerta. Bona sort!


# Funcionalitats

Aquest projecte consta de totes les funcionalitats demanades en la part bàsica: menú, crèdits, instruccions i jugabilitat. 

El nivell consta de 3 zones: asteroids, ruines i boss final. Els atacs són el ràpid i el carregat. Aquest últim atravesa i fa mal mentres
passa pels enemics, pel que és més efectiu quant més gran sigui l'enemic, degut a que té més temps de colisió a travès d'ell.

El force té 4 formes: 
- No hi és.
- Nivell 1: fa un tir endavant amb poca potència.
- Nivell 2: fa 3 tirs en forma de triangle de 90º que fan una mica més de dany.
- Nivell 3: fa 3 tirs igual que abans, però ara atravesen enemics, son més grans i fan més dany.

A partir d'aquí el force pot seguir aconseguint nivells fins a nivell 7, però l'únic efecte serà que es fa més gran.
Això pot ser útil degut a que augmenta el tamany de la seva col·lisió, la qual fa mal i bloqueja tirs.
Per millorar el force hi ha pick ups dispersats pel mapa, i també al matar a un cert nombre d'enemic apareixen alguns. 
Aquest mètode podria ser infinitament ampliable.

Hi ha 4 tipus d'enemics:
- Triple shooters: es poden moure a vegades cap enrera en direccions aleatòries mentres disparen 3 tirs en 90º.
- Wavers: es mouen cap a l'esquerra fent ones (normalment en conjunt).
- Wall crawlers: es troben en les parets i disparen en direcció cap al teu personatge ràpida i precisament.
- Shields: Tenen molta vida i a vegades es poden moure. La seva utilitat és servir de barricades i protegir zones.
	   Es podria crear una variant d'aquests que els tirs carregats no els atravessin, però no és el cas.

Finalment, el boss final dispara projectils que reboten en les esquines de la pantalla amb alta freqüència, i quan es troba a una certa vida
dispara una bola que persegueix al jugador constantment.

La tecla "f" puja el nivell del force, la "g" et torna invulnerable i les tecles 1, 2 i 3 et teletransporten als asteroids, ruines i boss respectivament.

En quant a les funcionalitats opcionals, he implementat so per a quasi totes les accions del joc i música. No he posat so als dispars dels enemics degut
a que penso que era massa i una mica confús pel jugador.

He posat 3 capes de background: una base amb estrelles, una segona amb altres estrelles que es desplacen a diferent velocitat, i una tercera amb planetes
que es desplaça encara més ràpidament. 

En quant a animacions, les he posat a enemics, player en moviment vertical, boss, force, explosions i so a la mort dels enemics, entre altres. 

Finalment, hi ha sistema de vides (3), les quals es veuen a la esquina superior esquerra. Cada vegada que mors, perds una vida i has de tornar a començar.
Si mors 3 vegades et surt la pantalla de game over amb so, i si passes el boss et surt la pantalla de victoria amb una cançó, i tornes al menú principal.
Aquest sistema de vides tindria més sentit si s'ampliés el joc i hi haguès multiples nivells, de tal manera que tornaries a començar a l'últim nivell i
no desde el principi, guardant el teu estat al entrar en aquest, mecanisme que està gairebé implementat. 
Podria fer també que al perdre una vida tornesis a l'últim checkpoint (asteroids, ruines o boss), però canviava massa l'enunciat de la pràctica.

En resum, he implementat totes les funcionalitats propostes menys el parpelleig del boss i l'animació d'entrada al nivell, i he implementat algunes extres
com el sistema de vides, pujada de número de vides quan mates a x enemics, diferents tipus de tirs, múltiples maneres de millorar el force etc. 
