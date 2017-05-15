/*!\file texte.c
 *
 * \brief Crédits qui défilent + oeil volant qui tournent autour
 * \author LAYEB AXEL ,axel.layeb@gmail.com
 * \date April 2017 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <SDL_image.h>
#define NOMBREDETEXTURE 4


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
void texte_init(void) {
  int i;
  SDL_Surface * t;
  int vp[4], w, h;
  static char * files[] = {"images/anim1/text.png", "images/anim1/background1.jpg", "images/anim1/aile.jpg", "images/anim1/eye4.jpg" };
  _pId  = gl4duCreateProgram("<vs>shaders/texte.vs", "<fs>shaders/texte.fs", NULL);
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

void texte_draw(void) {
  static GLfloat a0 = 0.0, defil =-10;
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
  /* BACK GROUND*/
  glBindTexture(GL_TEXTURE_2D, _tId[1]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 0, -20);  
    gl4duScalef(9, 7, 0.01);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
  /* FIN BACK GROUND */
  /*Fée*/
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.5, 0, -1, 0);
    gl4duTranslatef(0, 0, 5);  
    gl4duScalef(0.2, 0.2, 0.2);
    gl4duSendMatrices();
    /*OEIL*/
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
     //gl4duRotatef(a0*0.5, 0, -1, 0);
     gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN OEIL */

    /* AILE 1*/
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, 1, 1);
      gl4duTranslatef(0.5, 2, 0.5);  
      gl4duScalef(0.2, 1, 0.01);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN AILE 1*/
    /* AILE 2 */
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, -1, -1);
      gl4duTranslatef(-0.5, 2, 0.5);  
      gl4duScalef(0.2, 1, 0.01);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN AILE 2 */
  }gl4duPopMatrix();
  //glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
  /*Fin fée */

//////////////////////////////////////////////////

 /*Fée 2*/
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.5, 0, 1, 0);
    gl4duTranslatef(3, 2, 5);  
    gl4duScalef(0.2, 0.2, 0.2);
    gl4duSendMatrices();
    /*OEIL*/

  glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
     // gl4duRotatef(a0*0.5, 0, -1, 0);
      gl4duSendMatrices();
    }gl4duPopMatrix();

    gl4dgDraw(_sphere);
    /*Fin OEIL*/

  /* AILE 1*/
  glBindTexture(GL_TEXTURE_2D, _tId[2]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*20 , 0, 1, 1);
    gl4duTranslatef(0.5, 2, 0.5);  
    gl4duScalef(0.2, 1, 0.01);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
  /*FIN AILE 1*/
      /* AILE 2 */
      glBindTexture(GL_TEXTURE_2D, _tId[2]);
      gl4duPushMatrix(); {
        gl4duRotatef(a0*20 , 0, -1, -1);
        gl4duTranslatef(-0.5, 2, 0.5);  
        gl4duScalef(0.2, 1, 0.01);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_sphere);
      /*fin AILE 2 */
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
/*Fin fée 2*/


//////////////////////////////////////////////////

 /*Fée 3*/
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.5, 0, 1, 0);
    gl4duTranslatef(-2, -2, 4);
    gl4duScalef(0.2, 0.2, 0.2);
    gl4duSendMatrices();
    /*OEIL*/

  glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
     // gl4duRotatef(a0*0.5, 0, -1, 0);
      gl4duSendMatrices();
    }gl4duPopMatrix();

    gl4dgDraw(_sphere);
    /*Fin OEIL*/

  /* AILE 1*/
  glBindTexture(GL_TEXTURE_2D, _tId[2]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*20 , 0, 1, 1);
    gl4duTranslatef(0.5, 2, 0.5);  
    gl4duScalef(0.2, 1, 0.01);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
  /*FIN AILE 1*/
      /* AILE 2 */
      glBindTexture(GL_TEXTURE_2D, _tId[2]);
      gl4duPushMatrix(); {
        gl4duRotatef(a0*20 , 0, -1, -1);
        gl4duTranslatef(-0.5, 2, 0.5);  
        gl4duScalef(0.2, 1, 0.01);
        gl4duSendMatrices();
      } gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_sphere);
      /*fin AILE 2 */
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
/*Fin fée 3*/


///////////////////////////////////////////////////



/* TEXT 1 */
  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*10 , 1, -1, 0);
    gl4duTranslatef(0, defil, 0);  
    gl4duScalef(2, 5, 0.0001);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
/* FIN TEXT 1 */
    defil+= 0.01;
  a0 += 360.0 * dt / (6.0);
  //gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE);
  gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
 gl4dfSobel(0, 0, GL_FALSE);
}

