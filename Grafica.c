// Pulsoximetru Digital - Proiect PCLP 2
// Realizat de: Dănilă Ștefan-Flavian
// Partea Grafică
// Profesor coordonator: Damian Cătalin

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

int puls[100];
int spO2[100];
int n = 0; // Numarul de perechi de valori

float media(int valori[], int d) {
    float suma = 0;
    for (int i = 0; i < d; i++) {
        suma += valori[i];
    }
    if (d > 0)
        return suma / d;
        else return 0;
}

void display() {
    char numar[50]; // Alocam suficient spatiu pentru text
    float dx, ky, max;
    float mediaPuls = media(puls, n);
    float mediaSpO2 = media(spO2, n);

    dx = 1.9 / 2; // spatiul pe x pentru fiecare coloana - dx
    if (mediaPuls > mediaSpO2)max = mediaPuls;
     else max = mediaSpO2;
    ky = 1.8 / max; // factorul de scalare pe y

    glClearColor(1.0f, 1.0f, 1.0f, 0.05f); // Fundal alb
    glClear(GL_COLOR_BUFFER_BIT);

    // Coloana pentru media pulsului
    glColor3f(0.0f, 0.0f, 1.0f); // Albastru
    glBegin(GL_QUADS);
    glVertex2f(-0.75, -0.9);
    glVertex2f(-0.55, -0.9);
    glVertex2f(-0.55, -0.9 + mediaPuls * ky);
    glVertex2f(-0.75, -0.9 + mediaPuls * ky);
    glEnd();

    // Text pentru media pulsului
    // Folosim snprintf pentru a format textul într-un buffer de dimensiune fixa
    // + snprintf pentru securitate
    // Multumesc stackoverflow
    snprintf(numar, sizeof(numar), "Media Pulsului: %.2f", mediaPuls);
    glRasterPos2f(-0.75 + dx / 4, -1.0); // Ajustam pozitia textului
    for (int j = 0; j < strlen(numar); j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, numar[j]);

    // Coloana pentru media SpO2
    glColor3f(1.0f, 0.0f, 0.0f); // Rosu
    glBegin(GL_QUADS);
    glVertex2f(0.15, -0.9);
    glVertex2f(0.35, -0.9);
    glVertex2f(0.35, -0.9 + mediaSpO2 * ky);
    glVertex2f(0.15, -0.9 + mediaSpO2 * ky);
    glEnd();

    // Text pentru media SpO2
    // Din nou snprintf pentru formatare
    snprintf(numar, sizeof(numar), "Media SpO2: %.2f", mediaSpO2);
    glRasterPos2f(0.15 + dx / 4, -1.0); // Ajustam pozitia textului
    for (int j = 0; j < strlen(numar); j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, numar[j]);
    glFlush();
}

int main(int argc, char** argv) {
    FILE *data;
    data = fopen("data.txt", "r");
    if (data == NULL) {
        perror("Eroare la deschiderea fisierului!");
        return 1;
    }
    char line[100];
    // Citim fiecare linie din fisier
    while (fgets(line, sizeof(line), data)) {
        // Folosim sscanf pentru a citi valorile din linia curenta
        if (sscanf(line, "%d,%d", &puls[n], &spO2[n]) == 2) n++;}

    printf("Numarul total de perechi de valori citite: %d\n", n);
    fclose(data); // Inchidem fisierul

    glutInit(&argc, argv);
    glutCreateWindow("Grafic medii puls si SpO2");
    glutFullScreen(); // Fullscreen pentru frumusete
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
