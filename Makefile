#  Makefile 
#  Auteur : Farès BELHADJ
#  Email  : amsi@ai.univ-paris8.fr
#  Date   : 03/02/2014

SHELL = /bin/sh
# définition des commandes utilisées
CC = gcc
ECHO = echo
RM = rm -f
TAR = tar
MKDIR = mkdir
CHMOD = chmod
CP = rsync -R
# déclaration des options du compilateur
PG_FLAGS =
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_image
CPPFLAGS = -I. $(SDL_CFLAGS)
CFLAGS = -Wall -O3
LDFLAGS = -lm $(SDL_LDFLAGS) -lGL4Dummies

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	MACOSX_DEPLOYMENT_TARGET = 10.8
        CFLAGS += -I/usr/X11R6/include -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
        LDFLAGS += -framework OpenGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
else
        LDFLAGS +=  -L/usr/lib -L/usr/X11R6/lib -lGL -lGLU
endif

#définition des fichiers et dossiers
PROGNAME = demoHelper
PACKAGE=$(PROGNAME)
VERSION = 1.43
distdir = $(PACKAGE)-$(VERSION)
HEADERS = audioHelper.h animations.h 
SOURCES = audioHelper.c animations.c window.c base.c kids.c kids2.c texte.c chenille.c thomas.c
OBJ = $(SOURCES:.c=.o)
DOXYFILE = documentation/Doxyfile
EXTRAFILES = COPYING shaders/basic.vs shaders/mix.fs shaders/basic.fs	\
shaders/base.vs shaders/base.fs shaders/kids.vs shaders/kids.fs  shaders/thomas.vs shaders/thomas.fs shaders/chenille.vs shaders/chenille.fs \
shaders/texte.vs shaders/texte.fs	\
images/anim1/text.png images/anim1/background1.jpg  images/anim1/eye4.jpg\
images/anim1/aile.jpg \
images/anim2/background1.jpg images/anim2/jaune.jpg images/anim2/bleu.png images/anim2/rouge.png images/anim2/vert.jpeg images/anim2/block_A.png images/anim2/block_B.png images/anim2/block_C.png images/anim2/block_D.png images/anim2/smiley.jpeg\
images/anim3/background.jpg images/anim3/fraise.jpg images/anim3/chenille1.jpg images/anim3/chenille2.jpg\
images/anim3/thomas.png images/anim3/eye.jpg\


DISTFILES = $(SOURCES) Makefile $(HEADERS) $(DOXYFILE) $(EXTRAFILES)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

dist: distdir
	$(CHMOD) -R a+r $(distdir)
	$(TAR) zcvf $(distdir).tgz $(distdir)
	$(RM) -r $(distdir)

distdir: $(DISTFILES)
	$(RM) -r $(distdir)
	$(MKDIR) $(distdir)
	$(CHMOD) 777 $(distdir)
	$(CP) $(DISTFILES) $(distdir)

doc: $(DOXYFILE)
	cat $< | sed -e "s/PROJECT_NAME *=.*/PROJECT_NAME = $(PROGNAME)/" | sed -e "s/PROJECT_NUMBER *=.*/PROJECT_NUMBER = $(VERSION)/" >> $<.new
	mv -f $<.new $<
	cd documentation && doxygen && cd ..

clean:
	@$(RM) -r $(PROGNAME) $(OBJ) *~ $(distdir).tgz gmon.out core.* documentation/*~ shaders/*~ GL4D/*~ documentation/html
