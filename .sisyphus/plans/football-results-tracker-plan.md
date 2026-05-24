# Pracenje rezultata fudbalskih utakmica — Plan implementacije

## Cilj projekta
Napraviti C++ konzolnu aplikaciju za administratora koja omogucava unos i pracenje timova, igraca, utakmica, strijelaca, tabele i osnovnih statistika, uz spremanje/ucitavanje podataka iz tekstualnih datoteka.

## Opsti pristup
Sistem gradimo fazno. Prvo postavljamo stabilan model podataka i pravila vlasnistva memorije, zatim CRUD funkcionalnosti, pa logiku tabele i statistike. Pretraga, rad sa datotekama i visenitnost dolaze tek kada osnovna logika postane stabilna.

## Predlozena arhitektura

### 1. Domain/model sloj
Klase:
- `Osoba`
- `Igrac : Osoba`
- `Tim`
- `Strijelac`
- `Utakmica`

Odgovornost:
- cuvanje podataka
- osnovna pravila i veze medju entitetima

### 2. Repository sloj
Klase/moduli:
- `TimRepository`
- `UtakmicaRepository`
- po potrebi i repozitorij za igrace, ali jednostavnije je da igraci budu vezani za tim

Odgovornost:
- cuvanje kolekcija u memoriji
- pristup podacima po ID-u

### 3. Service sloj
Servisi:
- `TimService`
- `UtakmicaService`
- `TabelaService`
- `StatistikaService`
- `PretragaSortService`
- `PersistenceService`

Odgovornost:
- poslovna logika
- validacija unosa i operacija
- izracun tabele i statistike
- spremanje i ucitavanje podataka

### 4. UI/Menu sloj
Odgovornost:
- prikaz menija
- unos i ispis podataka
- pozivanje servisa

## Model i odnosi

### `Osoba`
Bazna klasa za zajednicke podatke, npr. ime i prezime.

### `Igrac : Osoba`
Sadrzi dodatne podatke kao sto su ID igraca, broj dresa, pozicija i pripadnost timu.

### `Tim`
Sadrzi:
- `idTima`
- `naziv`
- `grad`
- `trener`
- `godinaOsnivanja`
- listu igraca

### `Strijelac`
Predstavlja zapis o golu:
- `idStrijelca`
- `idUtakmice`
- `idIgraca`
- `minuta`
- `autoGol`

### `Utakmica`
Sadrzi:
- `idUtakmice`
- `idDomacina`
- `idGosta`
- `datum`
- `rezultatDomacin`
- `rezultatGost`
- `kolo`
- listu strijelaca

## Strukture podataka
- `std::vector` za liste timova, igraca, utakmica i strijelaca
- `std::map` za pristup po ID-u i agregirane statistike

## Dinamicka memorija
Posto je zadat uslov da se koriste `new/delete`, rano moramo definisati vlasnistvo objekata.

Predlozeni model vlasnistva:
- `TimRepository` posjeduje i brise timove
- `Tim` posjeduje i brise svoje igrace
- `UtakmicaRepository` posjeduje i brise utakmice
- `Utakmica` referencira timove i igrace preko ID-eva ili pokazivaca bez preuzimanja vlasnistva

Najveci rizici:
- curenje memorije
- duplo brisanje
- dangling pointer nakon brisanja povezanih objekata

## Validacija
Pravila koja moraju postojati od pocetka:
- naziv tima ne smije biti prazan
- naziv tima mora biti jedinstven
- igrac mora imati validne osnovne podatke
- broj dresa ne smije biti dupliran unutar istog tima
- utakmica ne smije imati isti tim kao domacina i gosta
- rezultat ne smije biti negativan
- timovi i igraci moraju postojati prije vezivanja za utakmicu
- broj unesenih golova i evidencija strijelaca moraju biti konzistentni

## Logika tabele i statistike
Tabela se ne treba rucno odrzavati na vise mjesta. Najstabilniji pristup je da se uvijek racuna iz svih postojecih utakmica.

Za svaki tim racunamo:
- odigrane utakmice
- pobjede
- nerijesene
- poraze
- dati golovi
- primljeni golovi
- gol razliku
- bodove

Dodatno:
- lista najboljih strijelaca
- historija utakmica odabranog tima
- osnovni statisticki pregledi

## Spremanje i ucitavanje podataka
Predlozeni fajlovi:
- `timovi.txt`
- `igraci.txt`
- `utakmice.txt`
- `strijelci.txt`

Redoslijed ucitavanja:
1. timovi
2. igraci
3. utakmice
4. strijelci

Format treba biti jednostavan i stabilan, npr. tekstualni zapisi odvojeni znakom `;`.

## Visenitnost
Zasebna nit za automatsko azuriranje tabele je trazena, ali je treba implementirati tek kada osnovni sistem bude stabilan.

Preporuka:
- `std::thread` za background obradu
- `std::mutex` za zastitu zajednickih podataka
- kontrolisan shutdown thread-a
- bez direktnog ispisa iz pozadinske niti

## Faze rada

### Faza 1 — Model i pravila memorije
Cilj:
- definisati klase
- definisati odnose medju klasama
- odrediti vlasnistvo memorije
- odrediti ID strategiju

Isporuka:
- jasan model domene
- dogovorena pravila brisanja objekata

### Faza 2 — Timovi i igraci
Cilj:
- dodavanje tima
- prikaz timova
- izmjena i brisanje tima
- dodavanje igraca u tim
- prikaz, izmjena i brisanje igraca

Zasto ova faza ide rano:
Utakmice zavise od postojanja timova i igraca.

### Faza 3 — Utakmice i strijelci
Cilj:
- unos utakmice
- izmjena utakmice
- brisanje utakmice
- evidentiranje strijelaca i minutaze

Ključno pravilo:
Rezultat utakmice i lista strijelaca moraju biti uskladjeni.

### Faza 4 — Tabela i statistika
Cilj:
- automatski izracun tabele
- pregled utakmica tima
- lista najboljih strijelaca
- osnovna statistika

### Faza 5 — Pretraga i sortiranje
Cilj:
- pretraga utakmica po timu, datumu i kolu
- sortiranje utakmica po datumu ili kolu
- dodatna pretraga po igracu ili timu

### Faza 6 — Datoteke
Cilj:
- spremanje svih podataka
- ucitavanje svih podataka
- validacija neispravnih ili nepotpunih zapisa

### Faza 7 — Pozadinsko azuriranje tabele
Cilj:
- zasebna nit koja nakon promjena osvjezava tabelu
- sigurna sinhronizacija pristupa podacima

## Redoslijed implementacije
Preporuceni redoslijed:
1. model klasa
2. vlasnistvo memorije i destruktori
3. repozitoriji
4. validacija
5. meni za timove i igrace
6. meni za utakmice i strijelce
7. tabela i statistika
8. pretraga i sortiranje
9. spremanje/ucitavanje
10. thread za automatsko azuriranje

## Verifikacija po fazama

### Nakon Faze 2
- test dodavanja vise timova
- test dodavanja igraca
- provjera duplikata naziva tima i broja dresa

### Nakon Faze 3
- test unosa vise utakmica
- zabrana utakmice istog tima protiv sebe
- provjera uskladjenosti rezultata i strijelaca

### Nakon Faze 4
- rucna provjera bodova i gol razlike
- provjera top strijelaca
- provjera pregleda utakmica jednog tima

### Nakon Faze 6
- save/load round-trip test
- test praznih fajlova
- test neispravnih linija u datoteci

### Nakon Faze 7
- test stabilnosti pri vise uzastopnih izmjena
- test sigurnog gasenja aplikacije

## Najveci rizici projekta
1. greske sa memorijom zbog rucnog `new/delete`
2. nekonzistentnost izmedju rezultata i strijelaca
3. brisanje entiteta koji su vec povezani s utakmicama
4. greske pri parsiranju i ucitavanju datoteka
5. race condition problemi kod pozadinske niti

## Preporuceni nacin rada
Necemo sve praviti odjednom. Radicemo po fazama i zatvarati svaku fazu tek kada osnovna logika i test scenariji budu stabilni. To je najbezbjedniji pristup za C++ projekat koji koristi rucno upravljanje memorijom i vise povezanih entiteta.

## Sljedeci konkretan korak
Prvi radni korak treba biti definisanje strukture projekta i osnovnih `.h/.cpp` fajlova za modele, repozitorije, servise i meni, bez ulaska odmah u sve funkcionalnosti.
