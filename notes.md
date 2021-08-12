- utiliser fleches pour bouger
- spawns des pommes aléatoires uniformes
- grille de 15x15
- taille du m_snake au départ : 4, on grandit à chaque fois qu'on mange une pomme
-

graphisme :

features :

- best m_score sur une session
- start avec une touche quelconque
- recommencer quand on a fini (best m_score stocké en dehors de la classe Game)
- settings : taille de la grille, taille de base du m_snake, vitesse de base du m_snake

Long term :

- serialization des données

```cpp
    if (score < 5) m_simulationSpeed = simulationTabSpeed[0];
    else if (score < 10)
        m_simulationSpeed = simulationTabSpeed[1];
    else if (score < 15)
        m_simulationSpeed = simulationTabSpeed[2];
    else if (score < 20)
        m_simulationSpeed = simulationTabSpeed[3];
    else if (score < 25)
        m_simulationSpeed = simulationTabSpeed[4];
    else if (score < 30)
        m_simulationSpeed = simulationTabSpeed[5];
    else if (score < 50)
        m_simulationSpeed = simulationTabSpeed[6];
```