#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
//#include <iostream>
#define MUS_PATH "run1.wav"

using namespace std;

//Prototipo de nuestra devolución de llamada de audio
// Ver la implementación para más información
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// Declaraciones de variables
static Uint8 *audio_pos; // Puntero global al búfer de audio que se va a reproducir
static Uint64 audio_len; // Longitud restante de la muestra que tenemos que jugar

GLuint texture[0];

// variables que representan direccion de la camara
float lx=0.0f,lz=-1.0f;

// XZ posicion de la camara
float x=0.0f,z=1.0f;

float angulo = 0.0f;
//movimiento carro
float move=-0.4;//variable que controla las movimientos del carrito.
static int lim1=30.0,lim2=14.0;//variables que identifican el rango que se tiene de cada obstaculo
static int lim1p=45.0,lim2p=45.0;
static float posx=-0.4, limn2=30.0,posxp=0.4,limn2p=45.0;//variables de las posiciones de los obstaculos.

static int x1=0,y=0,bd=0;//variables que capturan las posiciones conforme al eje z.
static int x1p=0,yp=0,bdp=0;

static int ex=0,ex2=0,ex1=0,exd=0,yd=0;//variables que controlan la explosion del carrito con los objetos.
float cplay=0.0f;
int cambiar= -29;//variable que controla cuando los obstaculos se cambian de posocion.

//VARIABLES
// variables que representan direccion de la camara



//int cambiar= -29;
//VARIABLES
//funcion del muneco de nieve
void muneco_nieve()
{  
  glPushMatrix();
  glColor3f(1.0f, 1.0f , 1.0f);
  //cuerpo muneco de nieve
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.9, 10, 10);
  glPopMatrix();
  
  glPushMatrix();
  //cabeza muneco de nieve
  glTranslatef(0.0f ,1.8f, 0.0f);
  glutSolidSphere(0.6, 10, 10);
  glPopMatrix();
  
  glPushMatrix();
  // ojo izquierdo
  glColor4f(0.0,0.0,0.0,0.2);
  glTranslatef(-0.2f ,1.9f, 0.6f);
  glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  // ojo derecho
  glTranslatef(0.2f ,1.9f, 0.6f);
  glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  // boca
  glTranslatef(0.0f ,1.6f, 0.6f);
  //glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  //sonrisa 
  glTranslatef(0.1f ,1.65f, 0.6f);
  //glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  //sonrisa
  glTranslatef(-0.1f ,1.65f, 0.6f);
  //glutSolidSphere(0.1, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  // pico
  glColor3f(0.84,0.54,0.125);
  //glRotatef (0,10.0,0.1,0.0);
  glTranslatef(0.0f ,1.6f, 0.6f);
  glutSolidCone(0.2, 1.0, 5, 5);
  glPopMatrix();
  
}

//funcion del obstaculo del cubo
void cubo()
{
  glColor3f(0.8f, 0.54f, 0.0125f);
  glTranslatef(0.0f ,0.75f, 0.0f);
  glRotatef (-90,10.0,0.1,0.0);
  glutSolidCube (0.8);
  glPushMatrix();
  glPopMatrix();
  glColor3f(1.0f, 1.0f , 1.0f);
}
//escena del desierto
void desierto()
{
  glTranslatef(0.0, -0.5, 0.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);  
  glVertex3f(-100.0f, 0.0f, -1000.0f);
  glTexCoord2f(1.0f, 1.0f); 
  glVertex3f(-100.0f, 0.0f, 500.0f);
  glTexCoord2f(0.0f, 1.0f);  
  glVertex3f( 100.0f, 0.0f, 500.0f);
  glTexCoord2f(0.0f, 0.0f);  
  glVertex3f( 100.0f, 0.0f, -1000.0f);
  glEnd();
}
//escena de la nieve
void nieve()
{
  glTranslatef(0.0, -0.5, 0.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);  
  glVertex3f(-100.0f, 0.0f, -1500.0f);
  glTexCoord2f(1.0f, 1.0f); 
  glVertex3f(-100.0f, 0.0f, 1000.0f);
  glTexCoord2f(0.0f, 1.0f);  
  glVertex3f( 100.0f, 0.0f, 1000.0f);
  glTexCoord2f(0.0f, 0.0f);  
  glVertex3f( 100.0f, 0.0f, -1500.0f);
  glEnd();
}
//funcion que controla el score del juego
void text(float puntaje, bool perderya, bool ganar)
{
  char text[50];
  puntaje=(puntaje*-1) - 1;
  float gano = puntaje;
  if(perderya==false)
    sprintf(text, "Score: %.0f", puntaje);
    
  else if ( perderya == true)   
  {
	  sprintf(text, "Usted ha Perdido su puntaje fue: %.0f", puntaje);
  }
  if(ganar == true)
  {
	      sprintf(text, "!!!En hora buena has ganado!!! puntaje : %.0f",gano);
	      
   }  
    glColor3f(1, 1, 1);
    glRasterPos3f( 0.8 , 2.0 , 0.1);
    for(int i = 0; text[i] != '\0'; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

//funcion de los obstaculos de los pinos.
void piramide()
{
  glColor3f(1.0f, 0.0f, 1.0f);
  glTranslatef(0.0f ,0.75f, 0.0f);
  glRotatef (-90,10.0,0.1,0.0);
  glutSolidCone(0.5, 1.0, 5, 5);
  glPushMatrix();
  glPopMatrix();
  glColor3f(1.0f, 0.0f , 1.0f);
}
//escena de la calle
void calle(float a,float b,float c,float d)
{
  glTranslatef(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);  
  glVertex3f(-1.0f, 0.0f, -a);
  glTexCoord2f(1.0f, 1.0f); 
  glVertex3f(-1.0f, 0.0f, b);
  glTexCoord2f(0.0f, 1.0f);  
  glVertex3f( 1.0f, 0.0f, c);
  glTexCoord2f(0.0f, 0.0f);  
  glVertex3f( 1.0f, 0.0f, -d);
  glEnd();
}
//escena de la grama
void grama()
{
  glTranslatef(0.0, -0.5, 0.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 0.0f);  
  glVertex3f(-100.0f, 0.0f, -500.0f);
  glTexCoord2f(1.0f, 1.0f); 
  glVertex3f(-100.0f, 0.0f, 500.0f);
  glTexCoord2f(0.0f, 1.0f);  
  glVertex3f( 100.0f, 0.0f, 500.0f);
  glTexCoord2f(0.0f, 0.0f);  
  glVertex3f( 100.0f, 0.0f, -500.0f);
  glEnd();
}
//funcion de los pinos laterales de la calle
void pinos()
{
  glColor3f(0.0f, 0.0f, 1.0f);
  glTranslatef(0.0f ,0.75f, 0.0f);
  glRotatef (-90,10.0,0.1,0.0);
  glutSolidCone(0.5, 1.0, 5, 5);
  glPushMatrix();
  glPopMatrix();
  glColor3f(1.0f, 1.0f , 1.0f);
}
//funcion del tronco de los pinos laterales
void tronco()
{
  glColor3f(0.8f, 0.54f, 0.125f);
  glTranslatef(0.0f ,0.75f, 0.0f);
  glRotatef (-90,10.0,0.1,0.0);
  glutSolidCube (5.5);
  glPushMatrix();
  glPopMatrix();
  glColor3f(1.0f, 1.0f , 1.0f);
}
//funcion de los captus en la escena del desierto
void  captus()
{
  glPushMatrix();
  glColor3f(0,1,0);
  glTranslatef(0.0f ,0.75f, 0.0f);
  glRotatef (0,10.0,0.1,0.0);
  glutSolidCube (1.5);
  glPopMatrix();
}
//funcion para el choque del carrito con los obstaculos
void explota()
{
  glPushMatrix();
  glColor3f(1.0f, 1.0f , 0.0f);
  glutSolidSphere (5.1, 100.3 ,4.5);
  glPopMatrix();  
}

void circuloplay(){
	
	 glPushMatrix();
     glColor3f(0.0f, 1.0f , 1.0f);
     //Delante izquierda
     // Rotacion de -30 grados en torno al eje y
     glRotated(1.0, 0.0, 1.0, 0.0);
     //negro
    glutSolidTorus (0.01, 0.4 ,8.5,90.0);
    glPopMatrix();
}
//funcion del play	
void play()
{	
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f,0.0f,1.0f);
    //VERTICES    
    glVertex3f(0.9f,0.45f,0.0f);
    glVertex3f(0.9f,-0.45f,0.0f);
    glVertex3f(1.65f,0.0f,0.0f);
    glEnd();
}
//funcion del pausar
void imgpausa()
{
    glBegin(GL_POLYGON);
    //cuadros izquierda
	glColor3f(1.5f,0.5f,1.0f);
    glVertex3f(0.5f,1.0f,0.0f);
    glVertex3f(-0.5,1.0f,0.0f);
    glVertex3f(-0.5f,-1.0f,0.0f);
    glVertex3f(0.5f,-1.0f,0.0f);
    glEnd();
}
//funcion de ganar	
void gano()
{
	 glBegin(GL_POLYGON);
     //cuadros izquierda
	glColor3f(1.5f,0.5f,1.0f);
    glVertex3f(1.0f,1.0f,0.0f);
    glVertex3f(-1.0,1.0f,0.0f);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glVertex3f(1.0f,-1.0f,0.0f);
    glEnd();
}

//funcion del carrito 
void carro()
{
  //Delante derecha
  glPushMatrix();
  glColor3f(0.0f, 0.0f , 0.0f);
  glTranslatef(0.5, -0.9, -0.2);
  // Rotacion de -30 grados en torno al eje y
  glRotated(90.0, 0.0, 1.0, 0.0);
  glutSolidTorus (0.1, 0.3 ,4.5,90.0);
  glPopMatrix();
    
  //llanta2
  glPushMatrix();
  glColor3f(0.0f, 0.0f , 0.0f);
  //Delante izquierda
  glTranslatef(-0.5, -0.9, -0.2);
  // Rotacion de -30 grados en torno al eje y
  glRotated(90.0, 0.0, 1.0, 0.0);
  //negro
  glutSolidTorus (0.1, 0.3 ,4.5,90.0);
  glPopMatrix();
    
  //llanta3
  glPushMatrix();
  glColor3f(0.0f, 0.0f , 0.0f);
  glTranslatef(0.5, -0.9, 1.2);
  // Rotacion de -30 grados en torno al eje y
  glRotated(90.0, 0.0, 1.0, 0.0);
  glutSolidTorus (0.1, 0.3 ,4.5,90.0);
  glPopMatrix();
    
  //llanta4
  glPushMatrix();
  glColor3f(0.0f, 0.0f , 0.0f);
  glTranslatef(-0.5, -0.9, 1.2);
  // Rotacion de -30 grados en torno al eje y
  glRotated(90.0, 0.0, 1.0, 0.0);
  glutSolidTorus (0.1, 0.3 ,4.5,90.0);
  glPopMatrix();
	
  //carroceria
  glPushMatrix();
  glColor3f( 1.0, 0.0, 0.0 );
  glutSolidCube(1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0, 0.0, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();
    
  //techo
  glPushMatrix();
  glColor3f( 0.0, 0.0, 1.0 );
  glTranslatef(0.0, 0.5, 0.0);
  glutSolidCube(0.7);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0, 0.5, 0.7);
  glutSolidCube(0.7);
  glPopMatrix();  
  
  glEnd();
  glFlush();
}
//funcion de la camara
void camara(int w, int h)
{

// Evitar una división por cero, cuando la ventana es demasiado pequeña
// (No permitir una ventana de alto 0).
if (h == 0)
h = 1;
float proporcion = w * 1.0 / h;

// Usando Matrix proyección
glMatrixMode(GL_PROJECTION);

// Reseteando la Matrix
glLoadIdentity();

// configurar el viewport para una ventana completa
glViewport(0, 0, w, h);

// Configurando las perspectivas.
gluPerspective(45.0f, proporcion, 0.1f, 100.0f);

// Regresar la vista a Modelview
glMatrixMode(GL_MODELVIEW);
}
static int a=1,ap=0, ganador=0;
static float d=0.0;
bool perd = false, ganastes = false;
//funcion que dibuja las escenas del juego y le funcionaminto al carrito
void escena(void)
{
  // Borrar Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Reseteando transformaciones
  glLoadIdentity();
  // Configurando camara

  if(a==1)
  {
	  SDL_PauseAudio(1);
	  z=d;
	  ex2;

  }
  
      if(ap==1)
  {
	  z=d;
	  ex2;
 
 
  }
  
  if(z<=-500)
  {  
	  z-=1.2;//velocidad del carrito
  }
  

	  
 else
  {
		  z-=0.6;//velocidad del carrito
  }
	  

 if(ceil(z)==cambiar)
  {
	cambiar -= 102;
  }
  else if(ceil(z)==(cambiar +52))
  {
  }
    if(z<=-1450.0 and z>=-1460.0)
  {
	  d=z;
	  a=1;
	  SDL_PauseAudio(1);
	   ganador=1; 
	   ganastes = true;    
  }

  if(z<=((lim1*-1)))
  {
	  lim1+=30;
	  lim2+=30;
	  limn2+=30.0;
  }
  
    if(z<=((lim1p*-1)))
  {
	  lim1p+=30;
	  lim2p+=30;
	  limn2p+=30.0;		
  }
  
  gluLookAt(0.0,    1.0f, z,0.0+lx, 1.0f, z+lz,0.0f,   1.0f, 0.0f);
  glPushMatrix();
    
  float p = -1.5-z;
  glTranslatef(move ,0.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.2,0.2,0.2);//se usa para el tamono del carrito
  carro();//llamada a la funcion carrito
  glEnd();
  glPopMatrix();
  glEnd();
  
  if(a==1)
  {
  glPushMatrix();
  float p = -1.5-z;
  glTranslatef(0.0 ,1.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.5,0.5,0.5);
  circuloplay();
  glEnd();
  glPopMatrix();
  glEnd();

  glPushMatrix();
  //float p = -1.5-z;
  glTranslatef(-0.46 ,1.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.4,0.4,0.4);
  play();
  glEnd();
  glPopMatrix();
  glEnd();
}

if(ap==1)
{
  glPushMatrix();
  float p = -1.5-z;
  glTranslatef(0.0 ,1.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.5,0.5,0.5);
  circuloplay();
  glEnd();
  glPopMatrix();
  glEnd();

  glPushMatrix();
  glTranslatef(-0.05 ,1.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.05,0.15,0.0);
  imgpausa();
  glEnd();
  glPopMatrix();
  glEnd();

  glPushMatrix();
  glTranslatef(0.05 ,1.0f,-p);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(0.05,0.15,0.0);
  imgpausa();
  glEnd();
  glPopMatrix();
  glEnd();
}
	
if(ganador==1)
{
  glPushMatrix();
  glTranslatef(0.05 ,1.0f,-p+2.6);
  glTranslatef(0.0, 0.5, -4.2);
  glScalef(100.0,100.0,0.0);
  glEnd();
  glPopMatrix();
  glEnd();
}

  // Aplicando textura de la calle
  glPushMatrix();
  glColor3f(1.0,1.0,1.0);
  texture[0] = SOIL_load_OGL_texture // cargar la textura de la calle
  (
       "905-1.jpg",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  calle(1500.0,1500.0,1500.0,1500.0);//llamada a la funcion de la calle
  glPopMatrix();   
  
  
  
// Aplicando textura de la escena de la grama
  texture[1] = SOIL_load_OGL_texture // cargar la imagen de la grama
  (
       "02.jpg",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  grama();//llamada  a la funcion  grama

   int contadorx = 0, colocarobjetos = 1;
   for(int i = 0; i < 1; i++)
  {
    for(int j=-17; j <= -1; j++) 
    {
		//cout<<contadorx<<endl;
		float it=(i*30.0), jt=(j * 30.0);
			glPushMatrix();
			glTranslatef(posx,-0.4,jt);
			piramide();
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(posxp,-0.4,jt+15);
			piramide();
			glPopMatrix(); 
     }
  }
  //aplicar la textura del desierto
  glColor3f(1,1,1);
  texture[4] = SOIL_load_OGL_texture // cargar la textura del desierto
   (
       "desert.jpg",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
   );

  glBindTexture(GL_TEXTURE_2D, texture[4]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  desierto();//llamada a la funcion del desierto

    //colocar los cubos en la calle
	for(int i = 0; i < 1; i++)
    {
		for(int j=-33; j <= -18; j++) 
        {
			float it=(i*15.0), jt=(j * 30.0);
			glPushMatrix();
			glTranslatef(posx,0.4,jt);
			cubo();
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(posxp,0.4,jt+15);
			cubo();
			glPopMatrix();
        }
    }
   //aplicar la textura de la nieve
   texture[2] = SOIL_load_OGL_texture // carga la textura de la nieve
   (
       "nieve.jpg",
       SOIL_LOAD_AUTO,
       SOIL_CREATE_NEW_ID,
       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
   );
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  nieve();//llamada  a la funcion de la nieve
   
   //coloca los munecos en la orilla de la calle
for(int i = 0; i < 1; i++)
  {
    for(int j=-49; j <= -34; j++) 
    {
		float it=(i*15.0), jt=(j * 30.0);
	    
	    glPushMatrix();
        glTranslatef(posx,0.95,jt);
        glScalef(0.4,0.4,0.4);
        muneco_nieve();
        glPopMatrix();
         
        glPushMatrix();
        glTranslatef(posxp,0.95,jt+15);
        glScalef(0.4,0.4,0.4);
        muneco_nieve();
        glPopMatrix();
    }
  }

  float pinosI=-1.3;//posicion de los pinos izquierda
  float pinosD=1.3;//posicion de los pinos derecha

  if(z<=10)
  {
	  // Dibujando arboles
      for(int j=-32; j < -1; j++) 
      {
		  glPushMatrix();
		  glTranslatef(pinosI,1.1,j * 15.0);
		  pinos();
		  glPopMatrix();
      
		  glPushMatrix();
		  glTranslatef(pinosD,1.1,j * 15.0);
		  pinos();
		  glPopMatrix();
      
		  glPushMatrix();
		  glTranslatef(pinosI,0.8,j * 15.0);
		  glScalef(0.01,0.4,0.1);
		  tronco();
		  glPopMatrix();
      
		  glPushMatrix();
		  glTranslatef(pinosD,0.8,j * 15.0);
		  glScalef(0.01,0.4,0.1);
		  tronco();
		  glPopMatrix();
	  }
  }

  if(z<=-400)
  {
	  // Dibujando captus
      for(int j=-30; j < -1; j++) 
      {
		  //tallo
		  glPushMatrix();
          glTranslatef(-3,0,(j * 15.0)-500);
          glScalef(0.4,1.6,0.4);
          captus();
          glPopMatrix();
          //mano
          glPushMatrix();
          glTranslatef(-3,1.4,(j * 15.0)-500);
          glScalef(1.5,0.2,0.4);
          captus();
          glPopMatrix();
          
          //izquierdo
          glPushMatrix();
          glTranslatef(-3.9,1.5,(j * 15.0)-500);
          glScalef(0.3,1.4,0.4);
          captus();
          glPopMatrix();
          
          //Derecho
          glPushMatrix();
          glTranslatef(-2.1,1.5,(j * 15.0)-500);
          glScalef(0.3,1.0,0.4);
          captus();
          glPopMatrix();
          
          //------ lado izquierdo escena -----      
          //tallo
		  glPushMatrix();
          glTranslatef(3,0,(j * 15.0)-500);
          glScalef(0.4,1.6,0.4);
          captus();
          glPopMatrix();
          //mano
          glPushMatrix();
          glTranslatef(3,1.4,(j * 15.0)-500);
          glScalef(1.5,0.2,0.4);
          captus();
          glPopMatrix();
          
          //izquierdo
          glPushMatrix();
          glTranslatef(3.9,1.5,(j * 15.0)-500);
          glScalef(0.3,1.4,0.4);
          captus();
          glPopMatrix();
          
          //Derecho
          glPushMatrix();
          glTranslatef(2.1,1.5,(j * 15.0)-500);
          glScalef(0.3,1.0,0.4);
          captus();
          glPopMatrix();
      }
  }
  if(z<=-900)
  {
	  // Dibujando muneco de nieve
      for(int j=-30; j < -1; j++) 
      {    
          glPushMatrix();
          glTranslatef(3.0,0,(j * 15.0)-1000);
          glScalef(0.8,0.8,0.8);
          muneco_nieve();
          glPopMatrix();
          
          glPushMatrix();
          glTranslatef(-3.0,0,(j * 15.0)-1000);
          glScalef(0.8,0.8,0.8);
          muneco_nieve();
          glPopMatrix();
      }
  }
  
  if(p>=0)
  {
	  x1=(limn2*-1);
	  y=((((p*-1)-1.699994)));
      //verificamos que el cubo (x) sea igual a la posicion actaul del caarrito (y), y que la posicion del trinagulo 
      //sea igual a la posicion del carrito en x (move). Si esta condicion se cumple significaria que choco, el juego se detendra.
      if(z<0)
      {
		  if(x1==(y-6) && posx == move)
	      {
				  perd = true;
				  SDL_PauseAudio(1);
			      glColor3f(0.0,0.0,0.0);
          
			      glPushMatrix();
			      ex=y-3;
			      glTranslatef(move,0.5,(ex));
			      glScalef(0.4,0.4,0.4);
			      explota();
			      glPopMatrix();
			      d=z;					
			      a=1;      
	        }
	
	  x1p=(limn2p*-1);
	  yp=((((p*-1)-1.699994)));
	  if(x1p==(yp-6) && posxp == move)
	      {
				  perd = true;
				  SDL_PauseAudio(1);
			      glColor3f(0.0,0.0,0.0);
          
			      glPushMatrix();
			      ex=y-3;
			      glTranslatef(move,0.5,(ex));
			      glScalef(0.4,0.4,0.4);
			      explota();
			      glPopMatrix();
			      d=z;					
			      a=1;          
	       }  
       }
}
  glColor3f(0.0,0.0,0.0);
  glTranslatef(-1.0, 0.9, -4.2);
  glTranslatef(-1.0, 0.9, z);
  text(z,perd,ganastes); 
  glutSwapBuffers();
}
//funcion que controla las funciones del carrito
void salir(unsigned char key, int x, int y)
{
static int contador=1;
  if (key == 114)
  {
	  a=0;
	  ap=0;
      perd = false;
      SDL_PauseAudio(0);
  }

  if (key == 32)
  {
	  ap=1;
	  d=z; 
      perd = false;
      SDL_PauseAudio(1);
  }
  
  if (key == 27)
    exit(0);	
}
//funcion que controla el carrito por medio del teclado
void keyboard(int key, int xx, int yy)
{
  float fraccion = 0.6f;

  switch (key) 
  {
    case GLUT_KEY_LEFT :
       move=-0.4;
     
    break;
    case GLUT_KEY_UP :
       a=1;
       d=1.0f;
       ap=0;
       perd = false;
       SDL_PauseAudio(1);
	   lx=0.0f,lz=-1.0f;
       // XZ posicion de la camara
       x=0.0f,z=1.0f;
       angulo = 0.0f;
       //movimiento carro
       move=-0.4;
       lim1=30.0,lim2=14.0;
       lim1p=45.0,lim2p=45.0;
       posx=-0.4, limn2=30.0,posxp=0.4,limn2p=45.0;
       x1=0,y=0,bd=0,x1p=0,yp=0,bdp=0;
       ex=0,ex2=0,ex1=0,exd=0,yd=0;
       cplay=0.0f;      
       cambiar= -29;
       perd = false;
    break;
	  
    case GLUT_KEY_DOWN :
       z=0;
	break;
	  
    case GLUT_KEY_RIGHT :
     move=0.4;
    break;

  }
}
//arranque de toto el juego
int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);//posicion de la ventana
  glutInitWindowSize(500,500);//tamano de la ventana
  glutCreateWindow("Motor Racing");//encabezado de la ventana
   glClearColor(0.2f, 0.5f, 1.0f, 0.5f);
   
  // sonido
  	// Inicializar SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
			return 1;

	// Variables locales
	static Uint32 wav_length; // Longitud de nuestra muestra
	static Uint8 *wav_buffer; // Buffer que contiene nuestro archivo de audio
	static SDL_AudioSpec wav_spec; // Las especificaciones de nuestra pieza de música

	/* Cargar el WAV */
	// Las especificaciones, la longitud y el búfer de nuestro wav se llenan
	if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL )
	{
	  return 1;
    }
	// Establecer la función de devolución de llamada
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	
	// Establecer nuestras variables estáticas globales
	audio_pos = wav_buffer ; // Copia el buffer de sonido
	audio_len = wav_length; // Copia la longitud del archivo

	/*Abrir el dispositivo de audio */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 )
	{
	  fprintf(stderr, "No se pudo abrir el audio: %s\n", SDL_GetError());
	  exit(-1);
	}

	/* Empezar a jugar */
	SDL_PauseAudio(0);

	// Esperar hasta que no jugamos
	while ( audio_len > 0 ) 
	{
		SDL_Delay(10);
		// register callbacks
  glutDisplayFunc(escena);
  glutReshapeFunc(camara);
  glutIdleFunc(escena);
  glutKeyboardFunc(salir);  glutSpecialFunc(keyboard);
  // OpenGL init
  glEnable(GL_DEPTH_TEST);
  //Para texturas
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glutMainLoop();
  return 1;
	}

	// Cerrar todo
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
}
//Función de devolución de llamada de audio
//Aquí tiene que copiar los datos de su buffer de audio en el
// Solicitud de búfer de audio (secuencia)
//Sólo debe copiar tanto como la longitud solicitada (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) 
{

	if (audio_len ==0)
		return;

	len = ( len > audio_len ? audio_len : len );
	//SDL_memcpy (stream, audio_pos, len); 					// Simplemente copie desde un buffer en el otro
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// Mezclar de un buffer a otro

	audio_pos += len ;
	audio_len -= len ;
}
