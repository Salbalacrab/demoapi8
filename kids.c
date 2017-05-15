/*!\file kids.c
 *
 * \brief Scène de chambre d'enfant + jouet qui bougent
 * \author LAYEB AXEL axel.layeb@gmail.com
 * \date April 2017 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <SDL_image.h>
#define NOMBREDETEXTURE 15


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
void kids_init(void) {
  int i;
  SDL_Surface * t;
  int vp[4], w, h;
  static char * files[] = {
    "images/anim2/background1.jpg",
    "images/anim2/rouge.png",
    "images/anim2/jaune.jpg",
    "images/anim2/vert.jpeg",
    "images/anim2/bleu.png",
    "images/anim2/block_A.png",
    "images/anim2/block_B.png",
    "images/anim2/block_C.png", 
    "images/anim2/block_D.png",
    "images/anim2/smiley.jpeg",
    "images/anim2/sol.jpg",
    "images/anim2/fenetre.png",
    "images/anim2/porte.png",
    "images/anim2/blanc.jpg",
    "images/anim2/carpet.jpg"
  };
  _pId  = gl4duCreateProgram("<vs>shaders/kids.vs", "<fs>shaders/kids.fs", NULL);
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
static GLfloat dirball = 0, movball = 0, coord =4, coord2 =7, coord3 =10,coord4 =13;

void kids_draw(void) {
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
  gl4duTranslatef(0, 0, -18);
  mat = gl4duGetMatrixData();

  MMAT4XVEC4(lumPos, mat, _lumPos0);
  glUseProgram(_pId);

  glUniform1i(glGetUniformLocation(_pId, "tex0"), 0);
  glUniform1i(glGetUniformLocation(_pId, "tex1"), 1);
  glUniform4fv(glGetUniformLocation(_pId, "lumPos"), 1, lumPos);
  glUniform1i(glGetUniformLocation(_pId, "specular"), 1);
  /* envoi de toutes les matrices stockées par GL4D */
  gl4duPushMatrix(); {
  gl4duRotatef(a0/5, 0, 1, 0);

  /* BACK GROUND*/
   //MUR NORD
  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 9, -20);  
    gl4duScalef(12,10, 0.02);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_quad);
   //MUR SUD
  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 9, 20);  
    gl4duScalef(12,10, 0.02);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

   //MUR OUEST

  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
    gl4duTranslatef(-20,9, 0);  
    gl4duScalef(0.02, 10,12);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
   //MUR EST


  glBindTexture(GL_TEXTURE_2D, _tId[0]);
  gl4duPushMatrix(); {
    gl4duTranslatef(20, 9, 0);  
    gl4duScalef(0.02, 10,12);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
  /* FIN BACK GROUND */

  /* GROUND*/
  glBindTexture(GL_TEXTURE_2D, _tId[10]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -5.5, 0);  
    gl4duScalef(15,0.02,15);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
  /* FIN GROUND */
/*ROOF */
 glBindTexture(GL_TEXTURE_2D, _tId[13]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 12, 0);  
    gl4duScalef(15,0.02,15);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
/*FIN ROOF */
 /* carpet*/
  glBindTexture(GL_TEXTURE_2D, _tId[14]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -5.4, 0);  
    gl4duScalef(2,0.02,2);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
  /* FIN carpet */
//fenetre
  glBindTexture(GL_TEXTURE_2D, _tId[11]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 4, -19.9);  
    gl4duScalef(3,3, 0.02);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);

//Porte
  glBindTexture(GL_TEXTURE_2D, _tId[12]);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 0, 19.9);  
    gl4duScalef(3,3, 0.02);
    gl4duSendMatrices();
  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);


 ////////////////////////////////////////////////

/* BALL 1 */

glBindTexture(GL_TEXTURE_2D, _tId[9]);
    gl4duPushMatrix(); {
 gl4duTranslatef(-1,movball,-2);  

     gl4duRotatef(a0*1, 1, 1, 1);
    gl4duScalef(0.8,0.8,0.8);
     gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
/* FIN BALL 1 */
/* BLOCK 1*/
  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
    gl4duRotatef(0 , 0, 57, 0);
    gl4duTranslatef(-1,-2,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 1 */
 /* BLOCK 2*/
  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
    //gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(0,1,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 2*/
 /* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(0,-1,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 4*/
  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
    //gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(-1,0,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 4*/

/* BLOCK 5*/
  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
    //gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,0,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 5*/
/* BLOCK 6*/
  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
    //gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,-3,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 6*/
 /* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,-2,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(0,-3,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,-4,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(-1,-4,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(-1,-5,1);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/

/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[4]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(-1,-5,1);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,-5,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/

/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,5,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(1,4,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
   // gl4duRotatef(a0*0.5 , 0, 0, 1);
    gl4duTranslatef(-1,4,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
    //gl4duRotatef(80 , 0, 0, 0);
    gl4duTranslatef(0,3,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
    //gl4duRotatef(80 , 0, 0, 0);
    gl4duTranslatef(1,2,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/
/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
    //gl4duRotatef(80 , 0, 0, 0);
    gl4duTranslatef(-1,2,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/

/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
    //gl4duRotatef(80 , 0, 0, 0);
    gl4duTranslatef(-1,5,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/

/* BLOCK 3*/
  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
    //gl4duRotatef(80 , 0, 0, 0);
    gl4duTranslatef(0,6,0);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);
 /* FIN BLOCK 3*/



/* BLOCKS qui tombe*/

//N1
 gl4duPushMatrix(); {
    gl4duTranslatef(-3,coord,1); 


  glBindTexture(GL_TEXTURE_2D, _tId[5]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.45 , 0, 1,1);
 
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);


   gl4duSendMatrices();
  } gl4duPopMatrix();

//N2
 gl4duPushMatrix(); {
    gl4duTranslatef(3,coord2,1); 


  glBindTexture(GL_TEXTURE_2D, _tId[7]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.55 , 1, 0,1);
 
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);


   gl4duSendMatrices();
  } gl4duPopMatrix();

//N3
 gl4duPushMatrix(); {
    gl4duTranslatef(-3,coord3,-1); 


  glBindTexture(GL_TEXTURE_2D, _tId[6]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.55 , 1, 1,0);
 
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);


   gl4duSendMatrices();
  } gl4duPopMatrix();

//N3
 gl4duPushMatrix(); {
    gl4duTranslatef(1,coord4,1); 


  glBindTexture(GL_TEXTURE_2D, _tId[8]);
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.55 , 1, 1,0);
 
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_cube);


   gl4duSendMatrices();
  } gl4duPopMatrix();
/////////////////////////////////////////////////////////////////

/*Spaceship */
gl4duPushMatrix(); {

    /*CORPS*/
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
    gl4duRotatef(a0*0.57, 0, 1, 0);
    gl4duTranslatef(2, -1, 5);  
    gl4duScalef(0.5, 0.15, 0.5);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN CORPS */

    /*HUBLO*/
    glBindTexture(GL_TEXTURE_2D, _tId[4]);
    gl4duPushMatrix(); {
    gl4duRotatef(a0*0.57, 0, 1, 0);
    gl4duTranslatef(2, -0.8, 5);  
    gl4duScalef(0.25, 0.25, 0.25);
    gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN HUBLO */
    /*antenne*/
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
    gl4duRotatef(a0*0.57, 0, 1, 0);
    gl4duTranslatef(2, -0.5, 5);  
    gl4duScalef(0.01, 0.3, 0.01);
    gl4duSendMatrices();
    /*fin antenne */


    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN HUBLO */
    /*antenne2*/
    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
        gl4duRotatef(a0*0.57, 0, 1, 0);
        gl4duTranslatef(2, 0, 5);  
        gl4duScalef(0.05, 0.05, 0.05);
        gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*fin antenne2 */

  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);
/* fin spaceship */

/*HELICO */
  gl4duPushMatrix(); {
    gl4duRotatef(a0*0.51, 0, -1, 0);
    gl4duTranslatef(2, 0.5, 4);  
    gl4duScalef(0.3,0.3,0.3);
    gl4duSendMatrices();
    
    /*CORPS*/
    glBindTexture(GL_TEXTURE_2D, _tId[1]);
    gl4duPushMatrix(); {
     gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN CORPS */
    /* rotor 1*/
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, 1, 0);
      gl4duTranslatef(0, 2, 0);  
      gl4duScalef(0.2, 0.2, 0.2);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN rotor 1*/


    /* PALE 1 */
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, 1, 0);
      gl4duTranslatef(0, 2.2, 1);  
      gl4duScalef(0.2, 0.01, 1);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN PALE 1 */

    /* PALE 2 */
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, 1, 0);
      gl4duTranslatef(0, 2.2, -1);  
      gl4duScalef(0.2, 0.01, 1);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN PALE 2 */

    /* queue 1*/
    glBindTexture(GL_TEXTURE_2D, _tId[4]);
    gl4duPushMatrix(); {
      gl4duTranslatef(1.8, 0, 0);  
      gl4duScalef(1.8, 0.5, 0.15);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN queue 1*/

    /* rotor 2*/
     glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
      gl4duTranslatef(4, 0, 0); 
    /* PALE 3 */
    glBindTexture(GL_TEXTURE_2D, _tId[3]);
    gl4duPushMatrix(); {
      gl4duRotatef(a0*20 , 0, 0, 1);
      gl4duTranslatef(0, 0, 0.3); 
      gl4duScalef(1,0.1,0.01);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN PALE 1 */ 
      gl4duScalef(0.25,0.25,0.25);
      gl4duSendMatrices();

    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN rotor 2*/
/* pied 1*/
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
      gl4duTranslatef(0, -1.8, -1);  
      gl4duScalef(1.25, 0.15, 0.15);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN pied 1*/

/* pied 2*/
    glBindTexture(GL_TEXTURE_2D, _tId[2]);
    gl4duPushMatrix(); {
      gl4duTranslatef(0, -1.8, 1);  
      gl4duScalef(1.25, 0.15, 0.15);
      gl4duSendMatrices();
    }gl4duPopMatrix();
    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
    gl4dgDraw(_sphere);
    /*FIN pied 2*/

  }gl4duPopMatrix();
  glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
  gl4dgDraw(_sphere);


    gl4duSendMatrices();
  }gl4duPopMatrix();
/////////////////*FIN HELICO *///////////////////////////////
//cube qui chute
//oord =4, coord2 =7, coord3 =10,coord4 =13;


if(coord <= -7)
    coord =7;
coord =coord - 0.05;

if(coord2 <= -7)
    coord2 =7;
coord2 =coord2 - 0.05;

if(coord3 <= -7)
    coord3 =7;
coord3 =coord3 - 0.05;

if(coord4 <= -7)
    coord4 =7;
coord4 =coord4 - 0.05;

//mouvement balle 
if(dirball ==1){
 movball = movball +0.05;
    if( movball >= 0){
        dirball=0;}
    }
else{
    movball = movball -0.07;
    if( movball <= (-3.5)){
        dirball=1;}
    }
//mouvement balle fin
  a0 += 360.0 * dt / (6.0);
  //gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE);
 // gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
 //gl4dfSobel(0, 0, GL_FALSE);
}

