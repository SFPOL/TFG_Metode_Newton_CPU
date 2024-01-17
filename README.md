# RayTracingToy
Pràctica 1 - GiVD 2022-23

## Equip:
**B02**
* Roberto Alvarado Chamatrin
* Júlia Virgili Mateu
* Pol Sánchez Forns
* Òscar Romero Alarcon

### Features (marqueu les que heu fet i qui les ha fet)
- Fase 0
    - [x] Background amb degradat
      - Júlia
    - [x] Color Shading
      - Pol
      - Oscar 
    - [x] Normal Shading
      - Pol
    - [x] Depth Shading
      - Pol
    - [x] Intersecció amb l'escena
      - Pol
      - Roberto
- Fase 1 Creació de nous objectes i interseccions (VIRTUALWORLD) 
    - [x] Hit Box
      - Júlia 
    - [x] Hit Triangle
      - Roberto 
    - [x] Hit Malla
      - Oscar
- Creació de REAL DATA
    - [x] FITTED PLANE: Pla de terra
      - Júlia 
    - [x] Transformacions Translació i Escalat amb gizmos esferes
      - Roberto i Oscar
    - [x] Gizmo de Box
      - Oscar i Pol
    - [x] Noves dades
      - Tots


- Parts opcionals: 
    - [x] Cilindres
      - Pol
    - [ ] Escena CSG 
  
    - [ ] Més d'una propietat en les dades reals
   
    - [ ] Mapeig de les dades reals en una esfera

- Fase 2
    - [x] Num. Samples i Gamma Correction
        - Pol
    - [x] Blinn-Phong
        - Oscar 
    - [x] Phong
        - Júlia
    - [x] Cell Shading
        - Pol
    - [x] Ombres
        - Roberto
    - [x] Recursivitat
        - Pol i Òscar
    - [x] Metall
        - Pol
    - [x] Transparent
        - Júlia
    - [x] Visualization mapping
        - Oscar

- Parts opcionals:
    - [x] Ombres segons el color del material transparent
        - Roberto

- Fase 3
  - [x] Material textura
      - Pol
  - [x] Proves amb dades reals d'accidents de Barcelona
      - Pol 
      - Oscar 
 
  
    
    
## Explicació de la pràctica    
  * **Organització de la pràctica**
    * Hem decidit treballar amb branques en el Github per a tenir un millor control del flux de treball. Per a cada Fase hem repartit els passos de manera més o menys equitativa i cadascú ha treballat en local fins que ha obtingut la implementació correcta d'un feature. Aleshores hem usat els Pull Request per a actualitzar la branca main. Cal dir, però, que hi ha hagut molta sinergia i no hem fet una repartició estricta de passos per persona. Per a manternir un control de la feina dels altres membres del grup hem utilitzat l'hora de laboratori, per tal de posar-nos al dia i compartir dubtes, a banda d'altres canals de comunnicació com WhatsApp.
  
  * **Decisions a destacar**
    * En un inici hem optat per a trobar solució als problemes opcionals a part dels passos necessaris, com en el cas de la implementació de l'objecte cilindre. Més endavant, però, hem hagut de prescindir de les extensions per manca de temps. Tot i així, cap al final hem pogut implementar algunes millores en les ombres d'objectes transparents: atenuació, color i allHits(). 
    * Algunes decisions més concretes de diferents parts del codi es troben a continuació:
      
    
## Screenshots de cada fase
### Fase 0

* Comencem per experimentar amb degradats, fent desplacamanet lineals en l'espai 3D de RGB i l'espai afí, per a familiaritzar-nos amb el funcionament dels mètodes de les diferents capes del codi: des de la vista amb MainWindow, passant pel controlador i al Ray Tracer, escena virtual, etc.

Pregunta b: Quina escena es té carregada? Qui la crea? Quin setup té la classe? Des d’on es crea?
A la classe Controller, està definit el setup i la escena, la escena està creada en el constructor per defecte del Controller. I el setUp es crea també al Controller amb els paràmetres que agafa de la finestra Main Window que ens apareix al correr el codi.

Pregunta c. Si prems els botó de Trace, es veu el degradat de la figura 2. Per què es veu aquest degradat? A quin mètode es calcula aquest color? 
Es veu el degradar de la figura dos ja que a la funció run() del Raytracer, es crida el mètode setPixel que pinta els pixels del color que li diem, i en aquest cas hem definit en el mètode rayPixel() de Raytracer que retorna un valor de color depenent del vector que simbolitza el raig de la càmera a aquell punt normalitzat, doncs, per això ens dona una gran varietat de colors.

![F0-P1](https://user-images.githubusercontent.com/90824282/228905979-33860d94-5e8b-43e1-b404-c6626bb84a9a.png)

Pregunta d. Si desactives el flag de background, i tornes a fer Trace, quin efecte et trobes? Per què? Pots veure com ha arribat el flag de background al teu RayTracer? 
Si es desactiva el flag de background es veu una imatge complement negra. Ja que el mètode RayPixel() calcula aquest color, però el fet de que el background no estigui activat fa que no entri al if, doncs el color que retorna és un vector amb tres 0 (com s’inizialitza) i doncs es veu negre. 

Pregunta e. En quina variable es troben els colors que es poden posar a la interfiere per definir el background? 
El mètode getDownBackground() i el mètode getTopBackground() de setup retorna el color que s’ha definit a la interfície, doncs es guarden a les variables col0 i col1.

![F0-P3e](https://user-images.githubusercontent.com/90824282/228905994-58da216a-2785-4d02-8803-82fac33bdea7.png)

Pregunta f. I si volguessis canviar els colors del degradat? On els canviaries? 
 
Per canviar els colors del degradat es canviarien a la paleta de la interfície, seleccionant els colors de Top Background i Down Background desitjats.

En aquest punt comencem amb la creació d'objectes per a l'escena virtual (esfera parametritzada):

Pregunta g. Si prems Trace, no la veuràs, per què? 

No la veiem perque el metode hit de Scene no invoca els hits de cada un dels objectes que hi ha a l'escena.

![F0-P3g](https://user-images.githubusercontent.com/90824282/228906033-8b174bd9-ea1c-4436-8a4b-2bd1bdfa8edc.png)

Pregunta h. Com hi pots accedir? A quina variable pots aconseguir l’estratègia a cridar? Com es crea aquesta estratègia des del menú? 

Es pot accedir a través del setUp creat a init() de la classe RayTracer. Aquest setUp l'aconseguim a través del Controller. Un cop selecciones el shadingStrategy que vols fer servir, passant pel Builder i pel Controller podem settejar un ShadingStrategy nou creant-lo gràcies a un mètode de ShadingFactory.

![F0-P3i](https://user-images.githubusercontent.com/90824282/228906048-32325f03-6f0a-4607-bcb3-e3794a0805f5.png)

Pregunta i. Com faràs per a crear una nova estratègia de shading?

Primer de tot crearem una nova classe anomenada NormalShading duplicant la classe ColorShading ja creada. Replicarem el que hem fet amb els headers. Un cop canviat els noms perque coincideixin amb la classe, afegirem NormalShading a tots els mètodes de ShadingFactory i importarem el header del NormalShading a aquesta classe.

![F0-P3j](https://user-images.githubusercontent.com/90824282/228906169-07c201b1-31e8-4f8c-aeb5-25107ca28dc9.png)

Pregunta j. Com aconsegueixes que els colors estiguin normalitzats?

Primer trobem la distància entre el punt on hem fet hit i la posició de la càmara. Aquesta distància la dividirem entre un factor prèviament escollit (en el cas de la imatge el factor és 3). El resultat de la divisó el posarem en les tres coordenades del vector color i el retornarem.

![F0-P3l(1)](https://user-images.githubusercontent.com/90824282/228906251-2821a5be-9fa4-46ce-9e5f-50422d796e1c.png)
![F0-P3l(2)](https://user-images.githubusercontent.com/90824282/228906256-102f93bf-191c-4b91-9610-7d8c8690542a.png)

Com podem veure, les imatges corresponents al setup per defecte i a l'escena virtual twoSpheres.json surten correctament.

Pregunta n. Prova a carregar ara el fitxer de dades reals data0.json que conté en l’array de data amb un punt situat a -2, -1 del món real i mostreja el valor de 0.5. Manté el mateix fitxer de setup de la visualització que has usat en el punt anterior. Per què veus només una esfera? On està situada a la teva escena? Amb quin radi? Per què? Per què és lila i no de color "kd": [0.7,0.6,0.5] com posa el fitxer? Ressegueix amb el debugger la creació de l’escena en aquest cas on el fitxer conté dades. 

Amb la versió final del codi, no només es visualitza una esfera ja que ja es fan els calculs amb el fitted plane, etc. Però amb el codi inicial aparéix una esfera. Es veu una esfera unitaria ja que al crear un gymos d’un fitxer data es genera inicialment l’objecte com a unitari. Després depenent de les translacions i escalats es modifica. Per axiò es troba al mig de la escena (i radi 1). Es lila degut a que el color depèn de la magnitud de l’escalat, que depen de la dada donada respecte de la resta de possible data real. En aquest cas el escalat val 1 ja que no es calcula (en el codi inicial). 

### Fase 1

El primer objecte a crear ha estat una Box, amb arestes alineades amb els eixos de coordenades. Aquesta es defineix a partir de dos vèrtexs i, per defecte, en té un en l'origen i un en (1,1,1).

![box_setupBox](https://user-images.githubusercontent.com/31898483/228953250-b2899276-07ae-4732-8788-364a604a7a4e.png)

Obtenim aquesta imatge amb el setupBox.json i el fitxer de escena virtual box.json

A continuació hem creat l'objecte `Triangle`, que està definit a partir de 3 vèrtexs en la nostra escena. Cal destacar que aquest objecte té un atribut que és el pla en el qual està contigut. Aquest s'utilitza a l'hora de fer el hit, ja que ens permetrà fer servir la normal del pla i el mètode de les coordenades baricèntriques. És a dir, calcularem el hit amb Triangle a partir del hit de Pla. Fent New Triangle obtenim el triangle amb vèrtexs $(0, 0, 0)$, $(1, 0, 0)$, $(0, 1, 0)$ (i `ColorShading`):

![triang](https://user-images.githubusercontent.com/31885913/228995300-a26f15b7-fd95-4429-ad4a-e9f01e7ce717.png)

S'ha creat una Mesh com a objecte per a la nostra escena. A la pregunta "Quan penses que és millor crear els triangles de la malla?" hem tingut diverses versions. S'ha d'intentar optimitzar l'espai de memòria usat, aleshores no es una bona pràctica haver de crear tots els triangles per cada crida de hit a mesh que es realitzi. En el nostre cas, hem creat els triangles només llegir els vèrtexs i a partir d'allà s'ha cridat al hit de triangle quan voliem veure el hit de Mesh, que conseqüentment crida al hit de pla que s'ha inicialitzat al crear el Triangle. Hi ha maneres d'optimitzar-ho com comprovant primer si intersecava amb una bounding box o bounding sphere i si no ho feia, no calia inicialitzar els triangles. És una implementació que podríem afegir més endavant. 

Pregunta: Referent a la BoundingBox i BoundingSphere, quina creus que teòricament és millor? Com funcionen en la realitat?
Depenent del que es busca, si només es crea una de cada, la bounding sphere sol ser més precisa. Però, hi ha tècniques en les que podem anar definint bounding spheres mes petites dins de la inicial, tal que optimitzariem el càlcul de hits per les zones de la box on no hi ha objecte.

Visualització de la Mesh de cube.obj:
(Utilitzant el fitxer meshExample.json i el setUpMesh.json veiem la següent Mesh cube (càmera a $(-1, -1, 0)$, Normal Shading i `maxDepth = 0`)):

<img width="500" alt="Captura de pantalla 2023-03-31 a les 1 32 03" src="https://user-images.githubusercontent.com/114871724/228987648-c0076bbf-ca45-447d-b329-9e2f29d62a21.png">




Per a l'apartat de Real Data, hem creat un objecte Fitted Plane, que ens serveix com a base per a representar dades amb gyzmos. A continuació en tenim una visualització a mode d'exemple:

![data10_setupFittedPlane](https://user-images.githubusercontent.com/31898483/228954119-e1b3180a-c144-415a-9547-1a481ba4bc9a.png)

S'ha utilitzat el setupFittedPlane.json i el Data File data10.json

Per a les transformacions geomètriques, el centre de l'objecte en l'escena virtual es calcula fent una translació segons els límits definits al món real del `.json` i el `fittedPlane` de l'escena virtual, seguint la fórmula $\frac{x-x_{min}}{x_{max}-x_{min}} = \frac{x_{mon}-x_{min_{mon}}}{x_{max_{mon}}-x_{min_{mon}}}$, en el cas de les $x$. Respecte a la mida, es fa un escalat segons el `minValue` i `maxValue` de l'atribut, afegint una petita epsilon $\epsilon$ de manera que quan el valor és mínim, l'objecte en l'escena se segueixi veient (no sigui infinitament petit).

### Fase 2

PAS 1:

![1SampleZoom](https://user-images.githubusercontent.com/90824282/228983334-9d301a58-c9ba-4cab-a1b4-48736b701801.png)
![10SampleZoom](https://user-images.githubusercontent.com/90824282/228983340-de4bcebe-5963-449e-bc4c-676c2fdf2514.png)

Si creem l'escena virtual twoSpheres.json amb el setup per defecte i fem zoom al resultat, ens surten les captures mostrades adalt. La primera és amb 'numSamples' = 1 i la segona amb 'numSamples' = 10.

Pregunta  2: On afegiries un atribut numSamples que defineixi el nombre de rajos per píxel i així controlar aquest fet? 

L'afegiriem al setup, com la resta d'atributs útils en els mètodes de la classe Raytracer.

Pregunta 3: On faràs aquesta correcció?

La correcció s'ha de fer una vegada obtingut el color a ensenyar, es a dir, en la funció run de RayTracer.

PAS 2:

![Cell1Light](https://user-images.githubusercontent.com/90824282/228947566-cbc7aed7-77c2-4328-9bc8-cc8c23c6eb31.png)

Aquesta foto ha estat creada usant 'New Sphere' i setupCellShading1Light.json

![Cell2Lights](https://user-images.githubusercontent.com/90824282/228947576-a2b4b361-8dfd-4c71-bf97-aede7646d931.png)

Aquesta altra ha estat creada usant 'New Sphere' i setupCellShading2Lights.json

![CellAvançat](https://user-images.githubusercontent.com/90824282/228947587-3b4f9a07-30c2-42c8-b5ac-f8370385f650.png)

Aquesta ultima imatge ha estat creada usant threeSpheres.json i setupThreeSpheresCell.json

Per al cellShading no hem hagut d'usar cap atribut extra que en el cas de Phong o BlinnPhong. En la segona imatge podem veure que per calcular el color agafem la llum que doni més directa al punt i calculem quanta intensitat li dona. La ultima imatge és CellShading en objectes Lambertians i Metalics

Per al shading Phong, només cal canviar els càlculs realitzats en el Blinn-Phong en la component especular. Enlloc d'usar el vector mig H per a calcular l'angle d'aquest amb la normal, utilitzem el vector R reflectit en la superfície amb normal N. H és una aproximació d'aquest, mes senzilla de computar. A banda, el shininess utilitzat (alpha o beta) serà diferent. A continuació es mostra com queda Phong si usem el mateix shininess que Blinn-Phong (10 per a aquestes esferes).

![twoSpheres_setupRenderTwoSpheresPhong](https://user-images.githubusercontent.com/31898483/228959177-2e896def-ebd5-4093-8f72-a7423ddf47c0.png)

S'ha usat l'escena twoSpheres.json amb setupRenderTwoSpheresPhong.json

PAS 3: Implementem les ombres. Per fer-ho, tenim un mètode `computeShadow()` a `ShadingStrategy` que retorna `factorOmbra`, que està entre 0 i 1. Retorna 0 si està a l'ombra d'aquella llum, i 1 en cas contrari. Els valors diferents de 0 i 1 es fan servir per atenuar l'ombra en objectes transparents. El valor es va reduint a mesura que el raig atravessa més distància del material, fins arribar a 0 en cas que hagi recorregut una distància `dmax`. Aquesta distància està actualment està hardcodejada i és de 1.2, però per ser més precisos hauria de dependre de l'objecte i es podria llegir del `.json`. Segons el shading, utilitzem diferents tècniques:
- Blinn-Phong: les components difosa i especular del color es multipliquen pel factor d'ombra.
- Phong: les components difosa i especular del color es multipliquen pel factor d'ombra.
- Color Shading: la component difosa del material es multiplica per la `Ia` de la llum si factor d'ombra és 0 i per `Id` en cas contrari, i per a cada llum. En cas que no hi hagi llums, es calcula com en la fase 0.
- Normal Shading i Depth Shading: igual que amb Color Shading, però en comptes de per la component difosa, es multiplica pel color que es fa servir en cada cas (veure fase 0).
- Cell Shading: igual que els dos últims punts però en aquest cas hi ha d'haver llums.

Si fem servir `setupRenderOneSphere.json` i `twoSpheres.json`, obtenim els següents shadows col·locant la càmera a $(3, 1, 2)$, en ordre de la llista de sobre i amb `MAXDEPTH = 0` pels shadings de la fase 0:

![bp_sha](https://user-images.githubusercontent.com/31885913/229006809-df03e940-9366-430e-8ffd-947d3bf69353.png)
![p_sha](https://user-images.githubusercontent.com/31885913/229006823-9f388e40-2027-4d1c-b9e9-4c977f4e2098.png)

![c_sha](https://user-images.githubusercontent.com/31885913/229008679-b591427b-0179-4433-9a5c-338678f393fa.png)
![n_sha](https://user-images.githubusercontent.com/31885913/229008690-981fa674-4886-414c-b291-98b3218749bb.png)

![d_sha](https://user-images.githubusercontent.com/31885913/229008733-4fca19ba-be2a-4ee8-b52e-120f2a04fc5c.png)
![t_sha](https://user-images.githubusercontent.com/31885913/229006873-3a050e4f-6543-40cd-ac39-eb9c4f57a130.png)

Podem crear escenes més interessants i veure les interaccions amb les ombres, com aquí (usant `setupRenderTwoSpheres.json` i `shadowScene.json`, amb càmera a `(-4, 4, -3)` i `vfov = 55º`). `MAXDEPTH` està a 1 i, encara que s'implementa al pas següent, l'hem fet servir aquí per crear una escena més interessant amb boles metàl·liques. No s'ha de posar més `MAXDEPTH` per no cremar l'escena, i es veuen les rugositats de `scatter()`:

![s_sc](https://user-images.githubusercontent.com/31885913/229016579-0d1a80e6-d9e6-4769-b56c-552b1c1335a2.png)

PAS 4: Per fer aquest pas, hem fet servir els mètodes `scatter()`, implementació la qual varia segons el material i ens permet generar rajos secundaris i obtenir-ne el color.

![TwoMAX1](https://user-images.githubusercontent.com/90824282/228984876-a7c4e237-2c6f-445b-b3c2-5f5f7824f5ae.png)

![TwoMAX3](https://user-images.githubusercontent.com/90824282/228984878-edd39daf-8233-4cd5-8b33-0fd0bc621764.png)

![TwoMAX10](https://user-images.githubusercontent.com/90824282/228984882-17184e31-d348-46b7-99f0-761266b4d0c1.png)

Aquestes imatges son els twoSpheres.json amb el setupRenderTwoSpheresF2.json. El valor de maxdepth es el que els diferència. D'adalt a avall tenen maxdepth 1, 3 i 10. Aqui es veu com afecta el lambertià amb diferents maxdepths amb l'entorn.

I quan fem que el background no aporti res de llum a l'escena obtenim les següents imatges. Ara el valor del maxdepth és 1 i 10 respectivament:

![TwoWoutBMAX1](https://user-images.githubusercontent.com/90824282/228985323-9245ba17-677e-4763-85a0-0dc6ac1e272e.png)

![TwoWoutBMAX10](https://user-images.githubusercontent.com/90824282/228985330-de2ec0af-02e7-436a-8e0e-31c660ae2142.png)

----------------------------------------------------------------------------------

![MetallMAX1](https://user-images.githubusercontent.com/90824282/228984012-a664b96b-ba27-4858-b202-2f6e9e297cc6.png)

![MetallMAX3](https://user-images.githubusercontent.com/90824282/228984143-1095ce0b-e59c-40cb-bdca-bcfa0698d1f8.png)

![MetallMAX10](https://user-images.githubusercontent.com/90824282/228984152-02fe5dca-c937-4607-aa24-beb708a69be2.png)

Aquestes imatges son els threeSpheres.json amb el setupThreeSpheresF2.json. El valor de maxdepth es el que els diferència. D'adalt a avall tenen maxdepth 1, 3 i 10. Aqui es mostra com es comporta el metall amb diferents Maxdepths.

PAS 5: Aquí veiem una escena amb una bola transparent. Hem utilitzat la ponderació per 1-colorScattered per a la coloració local obtinguda pel shading (en aquest cas Blinn-Phong), per així obtenir més transparència. En cas contrari, veiem una coloració més forta aportada pel Blinn, normalment blanca.

![spheresMetalTransp_setupRenderTwoSpheres](https://user-images.githubusercontent.com/31898483/228970383-09088b99-a22f-43c6-958d-85d4025100f5.png)

spheresMetalTransp.json amb setupRenderTwoSpheres.json

Aquí veiem una altra esfera transparent amb una nut més elevada (tipus aigua). La nut és llegida del json i apareix en un dels constructors per al material. Cal apuntar que també hem prés la decisió d'assignar el color del Background per als rajos que no interseccionen amb cap objecte, enlloc d'utilitzar el color global. D'aquesta manera, aconseguim que a "través" de l'esfera transparent es visualitzi més clarament el fons i obtenim un efecte més realista de transparència. De l'altra manera obteniem un color mes fosc.

![fourSpheres_setupRenderFourSpheres](https://user-images.githubusercontent.com/31898483/228970394-97636be3-c444-436b-896f-cb333a55fe36.png)

fourSpheres.json amb setupRenderFourSpheres.json

És interessant veure com podem obtenir visualitzacions més realistes amb diferents extensions. Per exemple, aquí tenim l'efecte "Bubble", obtingut a partir de generar una esfera interior amb radi negatiu, de mòdul lleugerament menor. Al usar un nombre mínim de profunditat de recursió (10 aprox.) obtenim:

![fourSpheresBubble_setupRenderFourSpheresBubble](https://user-images.githubusercontent.com/31898483/228973129-fbc063f5-718f-4e13-87f9-2e354d98bac2.png)

Amb fourSpheresBubble.json i setupRenderFourSpheres.json

En aquestes visualitzacions no hem usat ombres, però si aquestes s'activen veiem que es produeix una atenuació en el cas d'ombres d'objectes transparents, ja que enlloc de multiplicar per un factor d'ombra de 0 o de 1, utilitzem un valor float entre aquests dos:

Amb `setupRenderTwoSpheres.json`, `spheresMetalTransp.json` i jugant amb la càmera (posició `(-4, 4, 2)` i `vfov = 55º`), podem veure l'atenuació de l'ombra:

![at_sha](https://user-images.githubusercontent.com/31885913/229009747-c0f18e1c-f48e-4f78-844d-fc753d433abe.png)

De fet, hem implementat la part opcional en què l'ombra es pinta segons el color del material transparent, però per defecte el codi està comentat per fer les visualitzacions més similars a l'enunciat (està implementat en Phong i BlinnPhong). Si el descomentem i utilitzem la mateixa configuració, ho podem observar (posem la `Kt` de l'esfera transparent a `(1.0, 0.0, 0.0)` en aquest cas):

![at_c_sha](https://user-images.githubusercontent.com/31885913/229012725-e145db28-2701-4c3d-8c09-894246fd9d4b.png)

PAS 6:

Per tal d'obtenir aquesta visualització, s'ha agafat el data0.json com a fitxer de dades, s'ha comentat la línia del mètode object Maps que definia la translació en y i sha posat translacio_y = 0 (per tal de que floti) i s'ha posat maxDepth = 1 i Shadows en la interficie. El resultat és el següent:

<img width="602" alt="Captura de pantalla 2023-03-31 a les 1 51 40" src="https://user-images.githubusercontent.com/114871724/228989623-760cef02-414f-4416-8bd5-0b8f62897829.png">

Les altres proves amb altres fitxers de dades s'han vist que funcionen anteriorment (al ensenyar la visualització de realData).

### Fase 3

Per a obtenir les imatges d'aquesta fase, s'ha descomentat el tros de codi maxDepth == 0 del primer else if de la funció RayPixel de Raytracer per tal de que no afegeixi la llum que ve del background en els rajos secundaris.

<img width="431" alt="Captura de pantalla 2023-03-31 a les 1 43 37" src="https://user-images.githubusercontent.com/114871724/228988851-ea6169bd-d99a-4801-9f5b-219224a29c4c.png">

PAS 1:

![EuropaNoTexture](https://user-images.githubusercontent.com/90824282/228985524-e80dfa8a-301c-423b-aa42-6db322fc9a92.png)

Aquesta és la imatge que ens surt al aplicar tot el que es diu a l'apartat 1 del pas 1 de la fase 3.

Pregunta 1: Amb el fitxer de setup setupDataEuropa.json quina de les dues imatges obtens?

Obtenim la imatge de l'esquerra, tal i com mostrem més amunt. 

Pregunta 2: Qui calcularà les coordenades (u, v) del punt d’intersecció amb el pla?

Ho farà el mètode hit de la classe FittedPlane, ja que es ella qui té tota la informació necessaria per calcular-lo.

![EuropaTexture](https://user-images.githubusercontent.com/90824282/228985660-238e8439-2226-48a1-be4b-1cd7a1d823d3.png)

Si apliquem els fitxers que ens diuen en l'apartat 3 del PAS 1 de la fase 3 ens surt la imatge d'adalt.

Pregunta 3: Si a la mateixa escena d'abans es considera aquest nou material en el terra, usant el setup de setupDataEuropa.json, quina de les dues visualitzacions obtens? 

Obtenim la primera, amb les posicións de les boles igual que en el cas anterior, com hem mostrat.

PAS 2:

Mostrem les imatges demanades en aquest apartat:

![BCNZoom](https://user-images.githubusercontent.com/90824282/228985830-859477eb-e603-4b5d-9565-71c64a6b8f52.png)
 
![BCN](https://user-images.githubusercontent.com/90824282/228985861-18940a9a-f628-49b4-8fea-472bc81a8135.png)

PAS 3:

D’Open Street Map hem tret les coordenades d’un rectangle del mapa de Barcelona que inclou els districtes de Sants-Montjuïc, Les Corts i Eixample. Aleshores, utilitzant la referència [2], hem trobat un csv que proporcionava les dades dels accidents de trànsit a Barcelona a l’any 2022, amb el corresponent barri, carrer, districte, etc. Aleshores, utilizan codi en Python, hem recomptat els accidents per cada barri dels tres districtes mencionats i hem proporcionat un json que pugui mostrar la imatge que veiem a continuació. On surten 17 esferes de diferentes tamanys sobre de cada barri de Barcelona, representant les zones amb més influència d’accidents. Hem definit uns mínims i maxims de Data real tal que puguem obtenir unes esferes més o menys proporcionals a la image i sense diferencies enormes entre elles. 

El setup és el “setUpDataBCN.json” i el fitxer de dades “dadesBCNNos.json”.

<img width="374" alt="Captura de pantalla 2023-03-31 a les 1 39 56" src="https://user-images.githubusercontent.com/114871724/228988178-c896d809-cfd8-46c7-914d-ce9545505659.png">

### Opcionals

![Cylinder](https://user-images.githubusercontent.com/90824282/228941714-b5747ff9-3e03-4e13-96ab-7ec044404901.png)

Aquesta imatge ha estat aconseguida mitjançant 'New Cylinder' i setUpCylinder.json

![CylinderTort](https://user-images.githubusercontent.com/90824282/228941722-b9135902-0ae7-4643-879c-f3eeeaaeca7f.png)

Aquesta l'hem obtigut usant cylinderTort.json i setUpCylinderTort.json

![Axis](https://user-images.githubusercontent.com/90824282/228941732-70833614-b131-4b45-afa2-e5516132c2ad.png)

I aquesta última usem Axis.json i setUpAxis.json

Hem fet que el nostre programa pugui mostrar tot tipus de cilindres, amb l'orientació desitjada.


* **Més visualitzacions**:

   * Per totes les imatges, incluiu aqui la imatge obtinguda i els fitxers de dades i de configuració que heu fet servir
# TFG_Metode_Newton_CPU
