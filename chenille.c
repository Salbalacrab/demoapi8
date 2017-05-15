/*!\file chenille.c
 *
 * \brief chenille qui se déplace + mange des fraises
 * \author LAYEB AXEL axel.layeb@gmail.com
 * \date April 2017 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <SDL_image.h>
#define NOMBREDETEXTURE 6
#define VITESSEDEPLACEMENT 0.02


/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief position de la lumière relativement à la sphère éclairée */
static GLfloat _lumPos0[4] = {0, -100, 60, 0.5};
/*!\brief tableau des identifiants de texture à charger */
static GLuint _tId[NOMBREDETEXTURE] = {0};
/*!\brief pour les deux astres : terre et lune */
static GLuint _sphere = {0};
static GLuint _quad = 0;
static GLuint _cube = 0;

void chenille_init(void) {
  int i;
  SDL_Surface * t;
  int vp[4], w, h;
  static char * files[] = {"images/anim3/background1.jpg","images/anim3/fraise.jpg","images/anim3/chenille1.jpg","images/anim3/chenille2.jpg", "images/anim3/aile.jpg", "images/anim3/cocoon.jpg"};
  _pId  = gl4duCreateProgram("<vs>shaders/chenille.vs", "<fs>shaders/chenille.fs", NULL);
  glGetIntegerv(GL_VIEWPORT, vp);
  w = vp[2] - vp[0];
  h = vp[3] - vp[1];
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * h / w, 0.5 * h / w, 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
  glGenTextures(NOMBREDETEXTURE, _tId);
  for(i = 0; i < NOMBREDETEXTURE; i++) {
    glBindTexture(GL_TEXTURE_2D, _tId[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if( (t = IMG_Load(files[i])) != NULL ) {
#ifdef __APPLE__
      int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
      int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
      SDL_FreeSurface(t);
    } else {
      fprintf(stderr, "can't open file %s : %s\n", files[i], SDL_GetError());
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
  }
  _sphere = gl4dgGenSpheref(30, 30);
  _quad = gl4dgGenQuadf();
  _cube = gl4dgGenCubef();
}
  static GLfloat movchenillex = -5, movchenilley = -5;
  static GLfloat movchenillex2 = -5, movchenilley2 = -5;
  static GLfloat movchenillex3 = -5, movchenilley3 = -5;
  static GLfloat movchenillex4 = -5, movchenilley4 = -5;
  static GLfloat movchenillex5 = -5, movchenilley5 = -5;
  static GLfloat fraise1=0, fraise2=0,fraise3=0,fraise4=0, fraise5=0, aile =0, aile2=0;


  static float tempo1 =0;
  static float movchenillez =-5;
  void chenille_draw(void) {
  static GLfloat a0 = 0.0;

  static Uint32 t0 = 0, t;
  GLfloat dt = 0.0, lumPos[4], *mat;
  dt = ((t = SDL_GetTicks()) - t0) / 1000.0;
  t0 = t;

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  gl4duTranslatef(0, 0, -14);
  mat = gl4duGetMatrixData();

  MMAT4XVEC4(lumPos, mat, _lumPos0);
  glUseProgram(_pId);

  glUniform1i(glGetUniformLocation(_pId, "tex0"), 0);
  glUniform1i(glGetUniformLocation(_pId, "tex1"), 1);
  glUniform4fv(glGetUniformLocation(_pId, "lumPos"), 1, lumPos);
  glUniform1i(glGetUniformLocation(_pId, "specular"), 1);
  /* envoi de toutes les matrices stockées par GL4D */
///////////////////////////////////////////////////
  /* BACK GROUND*/
  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 0, -20);  
    gl4duScalef(9, 7, 0.01);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
  /* FIN BACK GROUND */

///////////////////////////////////////////////////
    /*FRAISES*/
    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
   // gl4duRotatef(a0, 0, 1, 1);
    gl4duTranslatef(0,0,fraise5);  
    gl4duScalef(0.2,0.2,0.2);
    gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);

    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
   // gl4duRotatef(a0, 0, 1, 1);
    gl4duTranslatef(5,0,fraise3);  
    gl4duScalef(0.2,0.2,0.2);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);

    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
    //gl4duRotatef(a0, 0, 1, 1);
    gl4duTranslatef(0,5,fraise2);  
    gl4duScalef(0.2,0.2,0.2);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);

    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
   // gl4duRotatef(a0, 0, 1, 1);
    gl4duTranslatef(0,-5,fraise4);  
    gl4duScalef(0.2,0.2,0.2);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);

    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
   // gl4duRotatef(a0, 0, 1, 1);
    gl4duTranslatef(-5,0,fraise1);  
    gl4duScalef(0.2,0.2,0.2);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN FRAISES */
///////////////////////////////////////////////////
    /*TETE*/
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
    gl4duTranslatef(movchenillex,movchenilley,0); 

    gl4duScalef(0.6,0.6,0.6);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN TETE */
    /* DEBUT CORPS 1 */
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
    gl4duTranslatef(movchenillex2,movchenilley2,0); 

    gl4duScalef(0.5,0.5,0.5);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /* FIN CORPS 1 */


    /* DEBUT CORPS 2 */
    gl4duPushMatrix(); {
    gl4duTranslatef(movchenillex3,movchenilley3,0); 
        gl4duScalef(0.6,0.6,0.6);
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
    //gl4duTranslatef(movchenillex3,movchenilley3,0); 
    

    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);


    /* AILE 1*/
    glBindTexture(GL_TEXTURE_2D, _tId[4]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*10 , 0, 1, 1);
      gl4duTranslatef(0.5, 2, 0.5);  
      gl4duScalef(0.2, aile, aile2);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN AILE 1*/
    /* AILE 2 */
    glBindTexture(GL_TEXTURE_2D, _tId[4]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*10 , 0, -1, -1);
      gl4duTranslatef(-0.5, 2, 0.5);  
      gl4duScalef(0.2, aile, aile2);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN AILE 2 */


    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);

    /* FIN CORPS 2 */

    /* DEBUT CORPS 3 */
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
    gl4duTranslatef(movchenillex4,movchenilley4,0); 

    gl4duScalef(0.5,0.5,0.5);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN CORPS 3 */

    /* DEBUT CORPS 4 */
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
    gl4duTranslatef(movchenillex5,movchenilley5,0); 

    gl4duScalef(0.6,0.6,0.6);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN CORPS 4 */
////////////////////////////////////////////////////
printf("%f \n", movchenillez);
if(tempo1 > 22 && tempo1 < 30){
if(tempo1 < 26)
movchenillez +=0.02;
if(tempo1 > 26)
movchenillez -=0.02;
//COCOON 5
  gl4duPushMatrix(); {
    gl4duTranslatef(0,movchenilley5,movchenillez);  
    gl4duRotatef(400 ,1,1,1);
        gl4duScalef(0.6,0.6,0.6);
    glBindTexture(GL_TEXTURE_2D, _tId[5]);
    gl4duPushMatrix(); {
        gl4duRotatef(45 ,0,1,0);  
        gl4duScalef(0.5,0.5,0.5);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

//COCOON 4
  gl4duPushMatrix(); {
    gl4duTranslatef(0,movchenilley4,movchenillez); 
    gl4duRotatef(-400 ,1,1,1); 
        gl4duScalef(0.5,0.5,0.5);
    glBindTexture(GL_TEXTURE_2D, _tId[5]);
    gl4duPushMatrix(); {
        gl4duRotatef(45 ,0,1,0);
        gl4duScalef(0.5,0.5,0.5);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

//COCOON 3
  gl4duPushMatrix(); {
    gl4duTranslatef(0,movchenilley3,movchenillez); 
    gl4duRotatef(400 ,1,1,1); 
        gl4duScalef(0.6,0.6,0.6);
    glBindTexture(GL_TEXTURE_2D, _tId[5]);
    gl4duPushMatrix(); {
        gl4duRotatef(45 ,0,1,0);  
        gl4duScalef(0.5,0.5,0.5);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

//COCOON 2
  gl4duPushMatrix(); {
    gl4duTranslatef(0,movchenilley2,movchenillez); 
    gl4duRotatef(-400 ,1,1,1); 
        gl4duScalef(0.5,0.5,0.5);
    glBindTexture(GL_TEXTURE_2D, _tId[5]);
    gl4duPushMatrix(); {
        gl4duRotatef(45 ,0,1,0);
        gl4duScalef(0.5,0.5,0.5);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

//COCOON
  gl4duPushMatrix(); {
    gl4duTranslatef(0,movchenilley,movchenillez);  
    gl4duRotatef(400 ,1,1,1); 
        gl4duScalef(0.6,0.6,0.6);

    glBindTexture(GL_TEXTURE_2D, _tId[5]);
    gl4duPushMatrix(); {
        gl4duRotatef(450 ,0,1,0); 
        gl4duScalef(0.5,0.5,0.5);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
}




///////////////////////////////////////////////////
if(tempo1 < 21.75){

//MOUVEMENT TETE
if( (-5 <= movchenilley) && (movchenilley <= 5) && (-5 <= movchenillex) &&( movchenillex <= 0))
    movchenilley = movchenilley + VITESSEDEPLACEMENT;

if( (5 <= movchenilley) && (movchenilley <= 6) && (-5 <= movchenillex) &&( movchenillex <= 5))
    movchenillex = movchenillex + VITESSEDEPLACEMENT;

if( (-5 <= movchenilley) && (movchenilley <= 6) && (5 <= movchenillex) &&( movchenillex <= 6))
    movchenilley = movchenilley - VITESSEDEPLACEMENT;

if( (-6 <= movchenilley) && (movchenilley <=-5) && (0 <= movchenillex) &&( movchenillex <= 6))
    movchenillex = movchenillex - VITESSEDEPLACEMENT; 

//MOUVEMENT CORPS 1
if( tempo1 > 0.9){
    if( (-5 <= movchenilley2) && (movchenilley2 <= 5) && (-5 <= movchenillex2) &&( movchenillex2 <= 0))
        movchenilley2 = movchenilley2 + VITESSEDEPLACEMENT;

    if( (5 <= movchenilley2) && (movchenilley2 <= 6) && (-5 <= movchenillex2) &&( movchenillex2 <= 5))
        movchenillex2 = movchenillex2 + VITESSEDEPLACEMENT;

    if( (-5 <= movchenilley2) && (movchenilley2 <= 6) && (5 <= movchenillex2) &&( movchenillex2 <= 6))
        movchenilley2 = movchenilley2 - VITESSEDEPLACEMENT;

    if( (-6 <= movchenilley2) && (movchenilley2 <=-5) && (0 <= movchenillex2) &&( movchenillex2 <= 6))
        movchenillex2 = movchenillex2 - VITESSEDEPLACEMENT;
}

//MOUVEMENT CORPS2
if( tempo1 > 1.8){
    if( (-5 <= movchenilley3) && (movchenilley3 <= 5) && (-5 <= movchenillex3) &&( movchenillex3 <= 0))
        movchenilley3 = movchenilley3 + VITESSEDEPLACEMENT;

    if( (5 <= movchenilley3) && (movchenilley3 <= 6) && (-5 <= movchenillex3) &&( movchenillex3 <= 5))
        movchenillex3 = movchenillex3 + VITESSEDEPLACEMENT;

    if( (-5 <= movchenilley3) && (movchenilley3 <= 6) && (5 <= movchenillex3) &&( movchenillex3 <= 6))
        movchenilley3 = movchenilley3 - VITESSEDEPLACEMENT;

    if( (-6 <= movchenilley3) && (movchenilley3 <=-5) && (0 <= movchenillex3) &&( movchenillex3 <= 6))
        movchenillex3 = movchenillex3 - VITESSEDEPLACEMENT;
}

//MOVEMENT CORPS3
if( tempo1 > 2.7){
    if( (-5 <= movchenilley4) && (movchenilley4 <= 5) && (-5 <= movchenillex4) &&( movchenillex4 <= 0))
        movchenilley4 = movchenilley4 + VITESSEDEPLACEMENT;

    if( (5 <= movchenilley4) && (movchenilley4 <= 6) && (-5 <= movchenillex4) &&( movchenillex4 <= 5))
        movchenillex4 = movchenillex4 + VITESSEDEPLACEMENT;

    if( (-5 <= movchenilley4) && (movchenilley4 <= 6) && (5 <= movchenillex4) &&( movchenillex4 <= 6))
        movchenilley4 = movchenilley4 - VITESSEDEPLACEMENT;

    if( (-6 <= movchenilley4) && (movchenilley4 <=-5) && (0 <= movchenillex4) &&( movchenillex4 <= 6))
        movchenillex4 = movchenillex4 - VITESSEDEPLACEMENT;
}

//MOUVEMENT CORPS4
if( tempo1 > 3.6){
    if( (-5 <= movchenilley5) && (movchenilley5 <= 5) && (-5 <= movchenillex5) &&( movchenillex5 <= 0))
        movchenilley5 = movchenilley5 + VITESSEDEPLACEMENT;

    if( (5 <= movchenilley5) && (movchenilley5 <= 6) && (-5 <= movchenillex5) &&( movchenillex5 <= 5))
        movchenillex5 = movchenillex5 + VITESSEDEPLACEMENT;

    if( (-5 <= movchenilley5) && (movchenilley5 <= 6) && (5 <= movchenillex5) &&( movchenillex5 <= 6))
        movchenilley5 = movchenilley5 - VITESSEDEPLACEMENT;

    if( (-6 <= movchenilley5) && (movchenilley5 <=-5) && (0 <= movchenillex5) &&( movchenillex5 <= 6))
        movchenillex5 = movchenillex5 - VITESSEDEPLACEMENT;
}
}

if(tempo1 > 3)
    fraise1=20;
if(tempo1 > 7)
    fraise2=20;
if(tempo1 > 12)
    fraise3=20;
if(tempo1 >17)
    fraise4=20;
if(tempo1 > 22){
    fraise5=20;
}
if(tempo1 > 28){
 aile2 = 0.01;
    if( aile < 4)
    aile = aile + 0.1;
}

tempo1 = tempo1 + 0.01;

  a0 += 360.0 * dt / (6.0);
  //gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE);
  gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
 gl4dfSobel(0, 0, GL_FALSE);
}

