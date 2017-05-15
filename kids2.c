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
#define NOMBREDETEXTURE 14


/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief position de la lumière relativement à la sphère éclairée */
static GLfloat _lumPos0[4] = {0, 100, 60, 0.5};
/*!\brief tableau des identifiants de texture à charger */
static GLuint _tId[NOMBREDETEXTURE] = {0};
/*!\brief pour les deux astres : terre et lune */
static GLuint _sphere = {0};
static GLuint _quad = 0;
static GLuint _cube = 0;
void kids2_init(void) {
  int i;
  SDL_Surface * t;
  int vp[4], w, h;
  static char * files[] = {
    "images/anim2/background1.jpg",//0
    "images/anim2/rouge.png",//1
    "images/anim2/jaune.jpg",//2
    "images/anim2/vert.jpeg",//3
    "images/anim2/bleu.png",//4
    "images/anim2/block_A.png",//5
    "images/anim2/block_B.png",//6
    "images/anim2/block_C.png", //7
    "images/anim2/block_D.png",//8
    "images/anim2/sol.jpg",//9
    "images/anim2/fenetre.png",//10
    "images/anim2/porte.png",//11
    "images/anim2/blanc.jpg",//12
    "images/anim2/carpet.jpg"//13
  };
  _pId  = gl4duCreateProgram("<vs>shaders/kids2.vs", "<fs>shaders/kids2.fs", NULL);
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
void kids2_draw(void) {
  static int i =0;
  static GLfloat a = 0;
  static GLfloat a0 = 0;
  static GLfloat a1 = 0.0;
  static GLfloat a2 = 0.0;
  static GLfloat a3 = 0.0;
  static int tempo0 = 0;
  static int tempo1 = 0;
  static int vague = 0;
  static GLfloat pencher = 0;
  static GLfloat tempo2 = 0;
  static GLfloat movRobotX = 0.0;
  static GLfloat rotRobotY = 0.0;
  static GLfloat movbras1 = 0.0;
  static GLfloat movbras2 = 0.0;
  static GLfloat movbras12 = 0.0;
  static GLfloat movbras22 = 0.0;
  static GLfloat movpieds = 0.0;
  static GLfloat movjambe1 = 0.0;
  static GLfloat movjambe2 = 0.0;
  static GLfloat movtorse = 0.0;
  static GLfloat movtete = 0.0;
  static GLfloat movbassin = 0.0;
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
  gl4duRotatef(a0/2, 0, 1, 0); //ROTATION DE L'ANGLE DE VUE
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
      glBindTexture(GL_TEXTURE_2D, _tId[9]);
      gl4duPushMatrix(); {
        gl4duTranslatef(0, -5.5, 0);  
        gl4duScalef(15,0.02,15);
        gl4duSendMatrices();
      }gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      /* FIN GROUND */
    /*ROOF */
     glBindTexture(GL_TEXTURE_2D, _tId[12]);
      gl4duPushMatrix(); {
        gl4duTranslatef(0, 12, 0);  
        gl4duScalef(15,0.02,15);
        gl4duSendMatrices();
      }gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
    /*FIN ROOF */
     /* carpet*/
      glBindTexture(GL_TEXTURE_2D, _tId[13]);
      gl4duPushMatrix(); {
        gl4duTranslatef(0, -5.4, 0);  
        gl4duScalef(2,0.02,2);
        gl4duSendMatrices();
      }gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);
      /* FIN carpet */
    //fenetre
      glBindTexture(GL_TEXTURE_2D, _tId[10]);
      gl4duPushMatrix(); {
        gl4duTranslatef(0, 4, -19.9);  
        gl4duScalef(3,3, 0.02);
        gl4duSendMatrices();
      }gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);

    //Porte
      glBindTexture(GL_TEXTURE_2D, _tId[11]);
      gl4duPushMatrix(); {
        gl4duTranslatef(0, 0, 19.9);  
        gl4duScalef(3,3, 0.02);
        gl4duSendMatrices();
      }gl4duPopMatrix();
      glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
      gl4dgDraw(_cube);

    /////////////////*DEBUT ROBOT *///////////////////////////////
        gl4duPushMatrix(); {
            gl4duRotatef(rotRobotY, 0, 1, 0);
            gl4duTranslatef(movRobotX,-0.5,0);
            gl4duPushMatrix(); {
                gl4duRotatef(pencher, 1, 0, 0);
                        gl4duRotatef(movtorse, 0, 1, 0);
            //ZONE HAUTE
                    gl4duPushMatrix(); { 
                        gl4duRotatef(movtete, 0, 1, 0);
                        gl4duTranslatef(0,2,0);
                        //TETE
                            gl4duPushMatrix(); {
                                glBindTexture(GL_TEXTURE_2D, _tId[1]);
                                gl4duPushMatrix(); {
                                    
                                    gl4duScalef(0.45,0.45,0.45);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);
                        //BOUCHE
                                glBindTexture(GL_TEXTURE_2D, _tId[2]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.3,0.1,0.2);
                                    gl4duTranslatef(0,-3,4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);

                                glBindTexture(GL_TEXTURE_2D, _tId[3]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.4,0.1,0.2);
                                    gl4duTranslatef(0,-3,4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);
                        //FIN BOUCHE
                                //OEIL 1
                                glBindTexture(GL_TEXTURE_2D, _tId[4]);
                                gl4duPushMatrix(); {
                                   // gl4duRotatef(a0*0.57, 0, 1, 0);
                                    gl4duScalef(0.2,0.2,0.2);
                                    gl4duTranslatef(3,3,4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);
                                //FIN OEIL 1
                                //OEIL 2
                                glBindTexture(GL_TEXTURE_2D, _tId[4]);
                                gl4duPushMatrix(); {
                                   // gl4duRotatef(a0*0.57, 0, 1, 0);
                                    gl4duScalef(0.2,0.2,0.2);
                                    gl4duTranslatef(-3,3,4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);
                                //FIN OEIL 2
                                //oreille
                                glBindTexture(GL_TEXTURE_2D, _tId[3]);
                                gl4duPushMatrix(); {
                                   // gl4duRotatef(a0*0.57, 0, 1, 0);
                                    gl4duScalef(0.05,0.3,0.05);
                                    gl4duTranslatef(-20,3,0);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);

                                glBindTexture(GL_TEXTURE_2D, _tId[2]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.10,0.10,0.10);
                                    gl4duTranslatef(-10,15,0);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);
                              
                                glBindTexture(GL_TEXTURE_2D, _tId[3]);
                                gl4duPushMatrix(); {
                                   // gl4duRotatef(a0*0.57, 0, 1, 0);
                                    gl4duScalef(0.05,0.3,0.05);
                                    gl4duTranslatef(20,3,0);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);

                                glBindTexture(GL_TEXTURE_2D, _tId[2]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.10,0.10,0.10);
                                    gl4duTranslatef(10,15,0);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);


                                //FIN oreille
                                glBindTexture(GL_TEXTURE_2D, _tId[4]);
                                gl4duPushMatrix(); {
                                   // gl4duRotatef(a0*0.57, 0, 1, 0);
                                    gl4duScalef(0.2,0.2,0.2);
                                    gl4duTranslatef(-3,3,4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);

                            gl4duSendMatrices();
                            }gl4duPopMatrix();
                        //TETE FIN
                        gl4duSendMatrices();
                    }gl4duPopMatrix();
            //ZONE HAUTE
            //ZONE MILIEUX
            gl4duPushMatrix(); {
               // gl4duRotatef(, 0, 1, 0);
                //BUSTE
                glBindTexture(GL_TEXTURE_2D, _tId[2]);
                gl4duPushMatrix();{
                    gl4duScalef(0.7,0.7,0.7);
                    gl4duTranslatef(0,0,0);
                    gl4duSendMatrices();
                }gl4duPopMatrix();
                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                gl4dgDraw(_cube);
                //BUSTE FIN
///////////////////////////BRAS 1//////////////////////////////////////////////
               /*Axe de rotation entre ((le bras et le coude) et l'avant bras) et l'épaule */
                gl4duPushMatrix(); {
                    gl4duTranslatef(-2,1.25,0);
                    if(vague ==0)
                    gl4duRotatef(movbras1, 1, 0, 0);
                    else
                    gl4duRotatef(movbras1, 0, 0, 1);
                    //EPAULE 1
                    glBindTexture(GL_TEXTURE_2D, _tId[4]);
                    gl4duPushMatrix();{
                        gl4duScalef(0.3,0.3,0.3);
                        gl4duSendMatrices();
                    }gl4duPopMatrix();
                    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                    gl4dgDraw(_sphere);
                    //EPAULE 1 FIN
                    /*Axe de rotation entre (le bras et le coude) et l'avant bras */
                    gl4duPushMatrix(); {
                        gl4duTranslatef(0,-1,0);
                        //BRAS 1
                        glBindTexture(GL_TEXTURE_2D, _tId[1]);
                        gl4duPushMatrix(); {
                            gl4duScalef(0.2,0.3,0.2);   
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                        gl4dgDraw(_cube);
                        //BRAS 1 FIN

                        /*Axe de rotation entre le bras et le coude */
                        gl4duPushMatrix();{
                        gl4duTranslatef(0,-1,0);
                        if(vague ==0)
                            gl4duRotatef(movbras12, 1, 0, 0);
                        else
                            gl4duRotatef(movbras12, 0, 0, 1);
                            //COUDE 1
                            glBindTexture(GL_TEXTURE_2D, _tId[4]);
                            gl4duPushMatrix(); {
                                gl4duScalef(0.3,0.3,0.3);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_sphere);
                            //COUDE 1 FIN

                            //BRAS 1.2
                            glBindTexture(GL_TEXTURE_2D, _tId[3]);
                            gl4duPushMatrix(); {
                                gl4duTranslatef(0,-1,0);
                                gl4duScalef(0.2,0.3,0.2);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_cube);
                            //BRAS 1.2 FIN

                            gl4duSendMatrices();
                        }gl4duPopMatrix();

                        gl4duSendMatrices();
                    }gl4duPopMatrix();

                    gl4duSendMatrices();
                }gl4duPopMatrix();
///////////////////////////FIN BRAS 1//////////////////////////////////////////////
///////////////////////////BRAS 2//////////////////////////////////////////////
/*Axe de rotation entre ((le bras et le coude) et l'avant bras) et l'épaule */
                gl4duPushMatrix(); {
                    gl4duTranslatef(2,1.25,0);
                        if(vague ==0)
                            gl4duRotatef(movbras2, 1, 0, 0);
                        else
                            gl4duRotatef(movbras2, 0, 0, 1);
                    //EPAULE 2
                    glBindTexture(GL_TEXTURE_2D, _tId[4]);
                    gl4duPushMatrix();{
                        gl4duScalef(0.3,0.3,0.3);
                        gl4duSendMatrices();
                    }gl4duPopMatrix();
                    glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                    gl4dgDraw(_sphere);
                    //EPAULE 2 FIN
                    /*Axe de rotation entre (le bras et le coude) et l'avant bras */
                    gl4duPushMatrix(); {
                        gl4duTranslatef(0,-1,0);
                        //BRAS 2
                        glBindTexture(GL_TEXTURE_2D, _tId[1]);
                        gl4duPushMatrix(); {
                            gl4duScalef(0.2,0.3,0.2);   
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                        gl4dgDraw(_cube);
                        //BRAS 2 FIN

                        /*Axe de rotation entre le bras et le coude */
                        gl4duPushMatrix();{
                        gl4duTranslatef(0,-1,0);
                        if(vague ==0)
                            gl4duRotatef(movbras22, 1, 0, 0);
                        else
                            gl4duRotatef(movbras22, 0, 0, 1);

                            //COUDE 2
                            glBindTexture(GL_TEXTURE_2D, _tId[4]);
                            gl4duPushMatrix(); {
                                gl4duScalef(0.3,0.3,0.3);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_sphere);
                            //COUDE 1 FIN

                            //BRAS 2.2
                            glBindTexture(GL_TEXTURE_2D, _tId[3]);
                            gl4duPushMatrix(); {
                                gl4duTranslatef(0,-1,0);
                                gl4duScalef(0.2,0.3,0.2);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_cube);
                            //BRAS 2.2 FIN

                            gl4duSendMatrices();
                        }gl4duPopMatrix();

                        gl4duSendMatrices();
                    }gl4duPopMatrix();

                    gl4duSendMatrices();
                }gl4duPopMatrix();
///////////////////////////FIN BRAS 2//////////////////////////////////////////////
                //gl4duTranslatef(0,0,0);
                gl4duSendMatrices();
                }gl4duPopMatrix();
            //ZONE MILIEU FIN
 gl4duSendMatrices();
                                }gl4duPopMatrix();
            //ZONE BASSE*******************************************************
                gl4duPushMatrix(); {
                gl4duRotatef(movbassin, 0, 1, 0);
                    //BASSIN
                        glBindTexture(GL_TEXTURE_2D, _tId[3]);
                        gl4duPushMatrix(); {
                            //gl4duRotatef(a0*0.57, 0, 1, 0);
                            gl4duScalef(0.45,0.40,0.45);
                            gl4duTranslatef(0,-4,0);
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                    gl4dgDraw(_cube);
                    //BASSIN FIN
///////////////////////////JAMBE 1//////////////////////////////////////////////
                    gl4duPushMatrix(); {
                        gl4duTranslatef(1,-2,0);
                        gl4duRotatef(movjambe1,-1,0,0);
                        //FESSE 1
                        glBindTexture(GL_TEXTURE_2D, _tId[4]);
                        gl4duPushMatrix();{
                            gl4duScalef(0.3,0.3,0.3);
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                        gl4dgDraw(_sphere);
                        //FESSE 1 FIN
                        gl4duPushMatrix();{
                            gl4duTranslatef(0,-1,0);
                            //JAMBE 1
                            glBindTexture(GL_TEXTURE_2D, _tId[2]);
                            gl4duPushMatrix(); {
                                gl4duScalef(0.2,0.4,0.3);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_cube);
                            //JAMBE 1 FIN
                            gl4duPushMatrix();{
                                gl4duTranslatef(0,-1,0);
                                gl4duRotatef(movpieds,0,-1,0);//rotule cheville 2
                                //JAMBE 1.2
                                glBindTexture(GL_TEXTURE_2D, _tId[4]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.3,0.3,0.3);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);
                                //JAMBE 1.2 FIN
                                //JAMBE 1.3
                                glBindTexture(GL_TEXTURE_2D, _tId[1]);
                                gl4duPushMatrix(); {
                                    gl4duTranslatef(0,-0.5,0);
                                    gl4duScalef(0.2,0.2,0.4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                //glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);
                                //JAMBE 2.3 FIN
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        gl4duSendMatrices();
                    }gl4duPopMatrix();
///////////////////////////FIN JAMBE 1//////////////////////////////////////////////
///////////////////////////JAMBE 2//////////////////////////////////////////////
                    gl4duPushMatrix(); {
                        gl4duTranslatef(-1,-2,0);
                        gl4duRotatef(movjambe2,-1,0,0);
                        //FESSE 2
                        glBindTexture(GL_TEXTURE_2D, _tId[4]);
                        gl4duPushMatrix();{
                            gl4duScalef(0.3,0.3,0.3);
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                        gl4dgDraw(_sphere);
                        //FESSE 2 FIN
                        gl4duPushMatrix();{
                            gl4duTranslatef(0,-1,0);
                            //JAMBE 2
                            glBindTexture(GL_TEXTURE_2D, _tId[2]);
                            gl4duPushMatrix(); {
                                gl4duScalef(0.2,0.4,0.3);
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                            gl4dgDraw(_cube);
                            //JAMBE 2 FIN
                            gl4duPushMatrix();{
                                gl4duTranslatef(0,-1,0);
                                gl4duRotatef(movpieds,0,1,0);//rotule cheville 2
                                //JAMBE 2.2
                                glBindTexture(GL_TEXTURE_2D, _tId[4]);
                                gl4duPushMatrix(); {
                                    gl4duScalef(0.3,0.3,0.3);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_sphere);
                                //JAMBE 2.2 FIN
                                //JAMBE 2.3
                                glBindTexture(GL_TEXTURE_2D, _tId[1]);
                                gl4duPushMatrix(); {
                                    gl4duTranslatef(0,-0.5,0);
                                    gl4duScalef(0.2,0.2,0.4);
                                    gl4duSendMatrices();
                                }gl4duPopMatrix();
                                glUniform1i(glGetUniformLocation(_pId, "specular"), 0);
                                gl4dgDraw(_cube);
                                //JAMBE 2.3 FIN
                                gl4duSendMatrices();
                            }gl4duPopMatrix();
                            gl4duSendMatrices();
                        }gl4duPopMatrix();
                        gl4duSendMatrices();
                    }gl4duPopMatrix();
///////////////////////////FIN JAMBE 2//////////////////////////////////////////////
                }gl4duPopMatrix();
            //ZONE BASSE*******************************************************
            gl4duSendMatrices();
        }gl4duPopMatrix();
        /////////////////*FIN ROBOT *///////////////////////////////
    gl4duSendMatrices();
    }gl4duPopMatrix();


  a0 += 360.0 * dt / (6.0);
  a += 360 *0.015 / 6;
if(tempo1 == 0){
  a1 +=0.5;
  if(a1 > 50)
    tempo1=1;
}

if(tempo2 < 50)
    tempo2 += 0.1;
if(tempo2 > 50)
    tempo2 += 10;
if(tempo2 > 3000)
    tempo2 = 0;
if(tempo1 == 1){
  a1 -= 0.5;
  if(a1 < -50)
    tempo1=0;
}

if(tempo0 != 1){
  a2 += 0.5;
  if(a2 > 20)
    tempo0=1;
}
if(tempo0 != 0){
  a2 -= 0.5;
    if(a2 < -20)
    tempo0=0;
}
//Mouvement Dance
//Lèvement de bras
if(a > 150 && a< 300){
movbras12-=0.5;
movbras22-=0.5;
}

//tour sur soit même
if(a > 300&& a < 350){
rotRobotY+=15.5;
}
//Deplacement crab allez
if( a > 350 && a < 400){
movRobotX += a2/100;
movpieds += a2;
}
//Se penche
if( a > 400 && a < 450)
pencher+=0.2;

//mov de bras
if(a > 450 && a < 500){
movbras1-= a2/10;
movbras2+= a2/10;
}
//Deplacement crab retour

if( a > 500 && a < 550){
movRobotX -= a2/100;
movpieds += a2;
}

//Se remet droit
if( a > 550 && a < 600)
pencher-=0.2;

//tourne son buste mais perd la tete
if( a > 600 && a < 650){
movtorse+=13;
movtete+=18;
}

//remet ses jambes
if( a > 650 && a < 741){
movbassin-=5;
movjambe2+=1;
movtete-=17.60;
}
//on remet la tete droite
if( a > 741 && a < 743)
    movtete=0;

//il essais encore pour les jambes
if( a > 741 && a <850) {
    if(movjambe2 >0)
    movjambe2-=1;
}
if( a > 849 && a <851) {
    movjambe2=0;
}
if( a > 850 && a <900) {
rotRobotY+=0.5;
movpieds += a2;
}

//Se penche sur le coté
if( a > 900 && a < 950)
pencher-=0.2;

if( a > 950 && a < 1000){
  movbras12 += 1.5;
  movbras22 += 1.5;
  movbras1 += 0.9;
  movbras2 -= 0.9;
    if(a> 999){
      movbras12 =0;
      movbras22 =0;
      movbras1 =0;
      movbras2 =0;
    }
}
//leve les bras pour faire la vague
if( a > 1000 && a < 1050){
  vague =1;
  movbras1 -= 1.75;
  movbras2 += 1.75;
}

//debut vague gauche
    if(a  >1050 && a <1100  ){
        movbras12 +=1.5;
    }
    if(a > 1100 && a < 1150){
        movbras12 -=4;
        movbras1 +=1.5;
    }
    if(a > 1150 && a < 1200){
        movbras12 +=2.5 ;
        movbras1 -=1.5;
        pencher-=0.1;
    }
    if(a  >1200 && a <1250  ){
        movbras22 +=2.5 ;
        movbras2 -=1.5;
        pencher-=0.1;
    }

    if(a > 1250 && a < 1300){
        movbras22 -=4;
        movbras2 +=1.5;
    }
    if(a > 1300 && a < 1350){
        movbras22 +=1.5;
    }

    if(a > 1350&& a < 1400){
    pencher+=0.5;
    }

    if(a > 1400&& a < 1700){
        if(i == 0){
            if(a3 < 50){
                a3+=1;
                movbras22-=2.5;
                movbras12+=2.5;
            }
            else
            i =1;
        } 
        if(i == 1){
            if(a3 >0){
            a3-=1;
            movbras22+=2.5;
            movbras12-=2.5;
            }
            else
            i=0;
        } 
  
}
  //gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE);
 // gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
 //gl4dfSobel(0, 0, GL_FALSE);
}

