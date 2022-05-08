# Implementace projektu ICP 2021/22
------------------------------------
Autori:
- Hung Do (xdohun00)
- David Kedra (xkedra00)

Prerekvizity
------------
- Make
- Cmake
- Qt5

Překlad a spuštění programu
---------------------------
Pro překlad a spuštění programu je v kořenovém adresáři přibalen soubor `Makefile`.
```bash
# pro preklad program
make

# pro preklad a spusteni programu
make run

# pro vygenerovani dokumentace
make doc

# pro odstraneni vygenerovanych souboru
make clean
```

Ovládání programu
-----------------
Po spuštění aplikace se uživateli nově vytvoří instance třídního diagramu. Z pravého panelu může uživatel vytvořit novou instanci
třídního elementu. Ten se následně objeví na plátně, kde s ním může uživatel přemisťovat. Po označení elementu může uživatel libovolně
upravovat atributy a relace třídního elementu. Pro uložení je potřeba kliknout na zelené tlačítko s fajfkou.

Sekvenční diagram se přidává v horním panelu v sekci `Sequence diagram` -> `Add`. Vytvoří se nová instance sekvenčního diagramu v novém
tabu. Tam si uživatel může libovolně přidávat objekty a zprávy mezi objekty. Všechny editace se dělají přes pravý panel.

Pro mazání jednotlivých elementu je potřeba je označit a stisknout klávesu **DELETE**. Zprávy v sekvenčním diagramu se mažou
na pravém panelu pomocí dvojkliku na přislušnou zprávu v seznamu zpráv.

Načítání a ukládání do souboru
------------------------------
Program povoluje i načítání a ukládání do souborů formátu JSON. To se řeší pomocí klávesových zkratek `Ctrl+S`
pro uložení souboru a `Ctrl+O` pro načtení souboru.

Použití knihovny třetích stran
------------------------------
- GoogleTest [https://github.com/google/googletest]