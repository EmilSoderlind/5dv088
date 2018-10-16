# Mfind Psudocode (Emil Söderlind)

* Skapa mutex-lås

* Hantera flaggor/input med getops (nrthr, filtyp, målfil)

* Skapa en kö

* Spara start-mappar i kö

* Skapa (nrthr - 1) trådar



* Tills kön är tom (Trådsäkert!)


    * Lås mutex
    * Ta en mapp från kön -> **directory**
    * Lås upp mutex

    * Addera mapp-räknare i tråden

    * dirStream = opendir("**directory**")

    * While(dirStream)

        * dirEnt = readdir(dirStream)

        * Lås mutex
        * Om filen är en mapp -> Lägg till i kö
        * Lås upp mutex

        * Om filen är målfil (Beroende på flaggor) -> Skriv ut

    * Stäng mapp