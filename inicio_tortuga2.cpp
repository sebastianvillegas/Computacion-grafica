// File: inicio_tortuga.cpp
// Authors: Iván Toro (201523548) and Sebastián Villegas (201533597)
// Created on March 06, 2018 - 10:00 AM

#include <stdio.h>
#include <stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include "string.h"
#include <unistd.h>

using namespace std;
// Variables rastro
//Numero de puntos
int np = 0;
//vectores para las coordenadas x, y, z.
float px [10000];
float py [10000];
float pz [10000];
//Matriz de modelado 
GLdouble mModel[16];

//Flag ejes
bool axis = false;
bool mostrarTortuga = true;

char strCommand[256];
bool command = false;

int dibujo = 1;

//Variables Look At
GLfloat X = 5.0, Y = 4.0, Z = 0.0;

// Dibujar una tortuga en 2D.
void drawTurtle(void) {
    
    // Se define el estado del color, es decir, el color con que se va a pintar.
    // Se selecciona el color rojo.
    glColor3f(1.0, 0.0, 0.0);
    // Se definen las cordenadas.
    double x[] = {0.0, -0.1, -0.15, -0.35, -0.45, -0.4, -0.3, -0.4, -0.35, -0.2,
    -0.35, -0.45, -0.35, -0.2, -0.15, -0.1, -0.1, -0.15, -0.2, -0.1, 0.0};
    double z[] = {-0.5, -0.55, -0.7, -0.75, -0.6, -0.45, -0.35, -0.2, 0.1, 0.25,
    0.3, 0.5, 0.6, 0.55, 0.45, 0.45, 0.6, 0.7, 0.8, 0.9, 0.95};
    // Se dibuja el contorno de la tortuga.
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 21; i++) {
        glVertex3f(x[i], 0.0, z[i]);
    }
    for (int i = 20; i >= 0; i--) {
        glVertex3f(x[i] * -1.0, 0.0, z[i]);
    }
    glEnd();
}

// Dibujar tortuga en 3D.

void drawSphereTurtle(void) {
    
    // Se define el estado del color, es decir, el color con que se va a pintar.
    // Se selecciona el color azul.
    glColor3f(1.28, 0.68, 0.56);
    // Se dibuja el cuerpo.
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glutWireSphere(0.4, 15, 15);
    glPopMatrix();
    // Se dibuja la cabeza.
    glColor3f(0.7, 2.05, 0.7);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.55);
    glutWireSphere(0.2, 15, 15);
    glPopMatrix();
    // Se dibujan las patas.
    glPushMatrix();
    glTranslatef(0.35, 0.0, 0.35);
    glutWireSphere(0.13, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.35, 0.0, 0.35);
    glutWireSphere(0.13, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.35, 0.0, -0.35);
    glutWireSphere(0.13, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.35, 0.0, -0.35);
    glutWireSphere(0.13, 15, 15);
    glPopMatrix();
    
}

// Dibujar ejes.

void ejes(void) {
    
    // Eje X.
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(5.0, 0.0, 0.0);
    glVertex3f(-5.0, 0.0, 0.0);
    glEnd();
    // Eje Y.
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 5.0, 0.0);
    glVertex3f(0.0, -5.0, 0.0);
    glEnd();
    // Eje Z.
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 5.0);
    glVertex3f(0.0, 0.0, -5.0);
    glEnd();
}

//Anadir puntos para el rastro.
void addPointToTrace() {
    int i;
    GLdouble m[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    // print the matrix
    printf("\nMatrix:\n");
    for (i = 0; i < 4; i++) {
        printf("Row %i: %f \t%f \t%f \t%f \n",
                i + 1, m[i + 0], m[i + 4], m[i + 8], m[i + 12]);
    }
    // if is the first point
    if (np == 0) { // add the first point
        px [0] = 0;
        py [0] = 0;
        pz [0] = 0;
        np++;
    }
    px [np] = m[0] * px [0] + m[4] * py [0] + m[8] * pz [0] + m[12];
    py [np] = m[1] * px [0] + m[5] * py [0] + m[9] * pz [0] + m[13];
    pz [np] = m[2] * px [0] + m[6] * py [0] + m[10] * pz [0] + m[14];
    printf("Point %i: %f \t%f \t%f \n",
            np, px[np], py[np], pz[np]);
    np++;
}

//Mostrar rastro
void displayTrace() {
    int i;
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < np; i++) {
        glVertex3f(px[i], py[i], pz[i]);
    }
    glEnd();
}

// La funcion encargada de dibujar o redibujar la ventana cada vez que sea necesario.

void display(void) {
    
    // Se establece el color de fondo de la pantalla, en este caso color blanco.
    glClearColor(1.0, 1.0, 1.0, 0.0);
    // Se borra el fondo de la ventana, los parametros son los buffers que borrara.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    //glLoadIdentity();
    //glMultMatrixd(mModel);
    
    
    if (mostrarTortuga){
        if (dibujo == 0) {
            
            drawTurtle();
        } else if (dibujo == 1) {
            
            drawSphereTurtle();
        }
    }
    
    if (axis) {
        
        ejes();
    }
    
    glPopMatrix();
    //glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
    //displayTrace();
    
    // Esta funcion define el intercambio entre los buffers posterios y anterior.
    glutSwapBuffers();
}

// Definir el "area de proyección" de la ventana. Desde donde se mira el objeto.
void reshape(int width, int height) {
    
    // Se difine el espacio en la ventana donde se puede pintar.
    // Los primeros parametros indican la parte superior izquierda del "lienzo"
    // relativo a la ventana, en este caso es el origen. Los siguientes dos parametros
    // indican el alcho y el alto del "lienzo".
    /*if (width < height) {*/
    
    glViewport(0, 0, width, height);
    /*} else {
     
     glViewport(0, 0, height, height);
     }*/
    // Se especifica la matriz de transformacion sobre la cual se trabajara,
    // en este caso se selecciona GL_PROJECTION afecta la prespectiva de proyección.
    glMatrixMode(GL_PROJECTION);
    // Carga como matriz de proyección la matriz identidad.
    glLoadIdentity();
    // Operar sobre la matriz de proyección, sus parametros definen el ángulo del campo
    // de vision en sentido vertical (Grados), la relacion entre la altura y la anchura
    // de la figura (Aspecto), el plano mas cercano de la camara, y el mas lejano,
    // respectivamente.
    gluPerspective(40.0, 1.0, 0.0, 150.0);
    // Ahora se especifica que se trabajara sobre la matriz de modelado.
    glMatrixMode(GL_MODELVIEW);
    // Nuevamente se carga la matriz identidad.
    glLoadIdentity();
    // Se define la transformacion sobre la vista inicial.
    // Los primeros tres parametros representan la distancia de los ojos del observador.
    // Los siguientes tres el punto de referencia al cual se observa, en este caso el origen.
    // Los tres ultimos la dirección del upVector, el vector que indica la direccion vertical.
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

char * insideRepeat(char* strCommandInside) {
    char *ini, *fin;
    ini = strchr(strCommandInside, '[');
    if (ini == NULL) return NULL;
    ini++;
    fin = strrchr(strCommandInside,']');
    if (fin == NULL) return NULL;
    strCommandInside[fin-strCommandInside]=0;
    return ini;
}

//Función que lee los comandos
void parseCommand(char* strCommandParse) {
    char *strToken0;
    char *strToken1;
    double val;
    char *repeatCommand;
    char *nextCommand;
    char parseCommandInit[256];
    int i;
    strToken0 = strtok(strCommandParse, " ");
    
    while ((strToken1 = strtok(NULL," ")) != NULL) {
        val = atof(strToken1);
        //usleep(100000);
        if (!strcmp("repeat",strToken0)) {
            
            repeatCommand = insideRepeat(strToken1 + strlen(strToken1) + 1);
            if (repeatCommand == NULL) return;
            nextCommand = repeatCommand + strlen(repeatCommand) + 1;
            for (i = 0; i < val; i++) {
                strcpy (parseCommandInit, repeatCommand);
                parseCommand(parseCommandInit);
            }
            strToken0 = strtok(nextCommand, " ");
            if (strToken0 == NULL) continue;
            continue;
        } else if (!strcmp("load",strToken0)) { // FORWARD
            freopen(strToken1, "r", stdin);
            char entrada[50000];
            char caracter;
            while(scanf("%c", &caracter)!=EOF){
                char aux[2] = " ";
                aux[0] = caracter;
                strcat(entrada, aux);
            }
            parseCommand(entrada);        
        } else if (!strcmp("fd",strToken0)) { // FORWARD
            glTranslatef(0.0, 0.0, val);
        } else if (!strcmp("bk",strToken0)) { // BACK
            glTranslatef(0.0, 0.0, -val);
        } else if (!strcmp("rt",strToken0)) { // RIGHT
            glRotatef(-val,0.,1.,0.);
        } else if (!strcmp("lt",strToken0)) { // LEFT
            glRotatef(val,0.,1.,0.);
        } else if (!strcmp("up",strToken0)) { // UP
            glRotatef(val,1.,0.,0.);
        } else if (!strcmp("dn",strToken0)) { // DOWN
            glRotatef(-val,1.,0.,0.);
        }
        strToken0 = strtok(NULL, " ");
        display();
    }
    // EXIT COMMAND MODE
    if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
        command = false;
        // HOME
    } else if (strToken0 != NULL && !strcmp("home",strToken0)) {
        glLoadIdentity();
    } else if (strToken0 != NULL && !strcmp("ht",strToken0)) { // FORWARD
        mostrarTortuga = false;
    } else if (strToken0 != NULL && !strcmp("st",strToken0)) { // FORWARD
        mostrarTortuga = true;
    }
}


// Esta funcion es la encargada de la interación con el teclado.
// Tambien se usara una funcion Callback pues en el programa se busca que la
// libreria maneje todos los eventos.

void keyboard(unsigned char key, int x, int y) {
    
    if (command) {
        if (key == 13) {
            strcat(strCommand, " ");
            if (strlen(strCommand) == 1) command = false;
            parseCommand(strCommand);
            strcpy(strCommand, "");
        } else {
            char strKey[2] = " ";
            strKey[0] = key;
            printf(strKey);
            strcat(strCommand, strKey);
        }
    }
    else {
        
        // Se ejecuta un case distinto dependiendo de la tecla precionada.
        switch (key) {
            case 'h':
                // Imprime en la consola información de como interactuar con el programa.
                printf("Help:\n");
                printf("1. 'c' = Toggle culling.\n");
                printf("2. 'q' or 'escape' = Quit.\n");
                printf("3. 'a' = Axis, (Red = x, green = y and blue = z).\n");
                printf("4. 'n' = Next figure (Switch Turtle 2D and Turtle 3D).\n");
                printf("5. '1' = Rotate in x.\n");
                printf("6. '2' = Rotate in y.\n");
                printf("7. '3' = Rotate in z.\n");
                printf("8. 'u' = + y.\n");
                printf("9. 'd' = - y.\n");
                printf("10. 'l' = - x.\n");
                printf("11. 'r' = + x.\n");
                printf("12. 'i' = Command mode.\n");
                printf("13. 'f' = Ejecutar instrucciones para la circunferencia.\n");
                break;
            case 'a':
                // Habilita o desabilita las ejes.
                axis = !axis;
                break;
            case 'c':
                // Habilita o desabilita las lineas posteriores.
                if (glIsEnabled(GL_CULL_FACE)) {
                    
                    glDisable(GL_CULL_FACE);
                } else {
                    
                    glEnable(GL_CULL_FACE);
                }
                break;
                //
            case 'n':
                // Cambia el dibujo que se pintara.
                if (dibujo == 1) {
                    
                    dibujo = 0;
                } else {
                    
                    dibujo++;
                }
                break;
            case '1':
                // Rota en el eje x.
                glRotatef(1.0, 1.0, 0.0, 0.0);
                break;
            case '2':
                // Rota en el eje y.
                glRotatef(1.0, 0.0, 1.0, 0.0);
                break;
            case '3':
                // Rota en el eje z.
                glRotatef(1.0, 0.0, 0.0, 1.0);
                break;
            case 'u':
                Y += 0.1f;
                glLoadIdentity();
                gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
                break;
            case 'd':
                Y -= 0.1f;
                glLoadIdentity();
                gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
                break;
            case 'l':
                X -= 0.1f;
                glLoadIdentity();
                gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
                break;
            case 'r':
                X += 0.1f;
                glLoadIdentity();
                gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
                break;
            case 'i':
                command = true;
                break;
            case 'f': 
                freopen("in", "r", stdin);
                char entrada[50000];
                char caracter;
                while(scanf("%c", &caracter)!=EOF){
                    char aux[2] = " ";
                    aux[0] = caracter;
                    strcat(entrada, aux);
                }
                parseCommand(entrada);
                break;
                
            case 'q': 
            case 27:
                // Sale de la aplicación.
                exit(0);
                break;
        }
        
    }
    // Esta función da la indicación a la GLUT que es necesario redibujar la ventana.
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    
    // Inicializar la ventana GLUT, los parametros son los mismo la función main.
    glutInit(&argc, argv);
    printf("Help:\n");
    printf("1. 'c' = Toggle culling.\n");
    printf("2. 'q' or 'escape' = Quit.\n");
    printf("3. 'a' = Axis, (Red = x, green = y and blue = z).\n");
    printf("4. 'n' = Next figure (Switch Turtle 2D and Turtle 3D).\n");
    printf("5. '1' = Rotate in x.\n");
    printf("6. '2' = Rotate in y.\n");
    printf("7. '3' = Rotate in z.\n");
    printf("8. 'u' = + y.\n");
    printf("9. 'd' = - y.\n");
    printf("10. 'l' = - x.\n");
    printf("11. 'r' = + x.\n");
    printf("12. 'i' = Command mode.\n");
    printf("13. 'f' = Ejecutar instrucciones para la circunferencia.\n");
    // Se define como se va a pintar en la ventana, los parametros son "Flags".
    // GLUT_RGB: Se usara el modelo Red-Green-Blue para definir el color.
    // GLUT_DEPTH: Indica que se usara el buffer de profundidad.
    // GLUT_DOUBLE: Señala que se usara un doble buffer.
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    // Se define el tamaño de la ventana, alto y ancho, en pixeles.
    glutInitWindowSize(1000, 1000);
    // Posicion, relativo a la pantalla, donde se desplegara la ventana.
    glutInitWindowPosition(0, 0);
    // Se crea la ventana, el argumento es el titulo.
    glutCreateWindow("Tortuga");
    
    
    // Las siguientes son funciones de tipo Callback, es decir, la libreria las ejecuta
    // su parametro, en este caso son funciones, cada vez que lo considere necesario.
    // Se ejecuta la funcion "display".
    glutDisplayFunc(display);
    // Se ejecuta la funcion "keyboard".
    glutKeyboardFunc(keyboard);
    // Se ejecuta la funcion "reshape".
    glutReshapeFunc(reshape);
    // Esta funcion es la encargada de dar el control de flujo del programa a la libreria
    // para que esta pueda ejecutar las funciones Callback cada vez que ocurra un evento.
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
    glPopMatrix();
    glutMainLoop();
    
    return 0;
}
