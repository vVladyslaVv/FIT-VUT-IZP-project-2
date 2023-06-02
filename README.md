# FIT-VUT-IZP-project-2

FIT VUT v Brne IZP project 2 (cluster)

Projekt 2 - Práce s datovými strukturami

# Popis projektu

Vytvořte program, který implementuje jednoduchou shlukovou analýzu, metodu nejbližšího souseda (angl. single linkage).

Smyslem projektu není studium shlukových analýz. Pro projekt bude stačit následující popis (zdroj Wikipedia): Shluková analýza (též clusterová analýza, anglicky cluster analysis) je vícerozměrná statistická metoda, která se používá ke klasifikaci objektů. Slouží k třídění jednotek do skupin (shluků) tak, že jednotky náležící do stejné skupiny jsou si podobnější než objekty z ostatních skupin.

Shlukovou analýzu provádějte na dvourozměrných objektech. Každý objekt je identifikován celým číslem. Objekty jsou uloženy v textovém souboru.

Při implementaci můžete pro vizualizaci a porozumění objektů používat tuto jednoduchou aplikaci, která vykresluje a obarvuje vámi vygenerované shluky.

# Metoda nejbližšího souseda

Metoda nejbližšího souseda vybírá ke spojení vždy dva shluky, které mají k sobě nejblíže. Vzdálenost dvou shluků je rovna nejmenší vzdálenosti libovolných dvou objektů z obou shluků. 
