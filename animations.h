/*!\file animations.h
 *
 * \brief Votre espace de liberté : c'est ici que vous pouvez ajouter
 * vos fonctions de transition et d'animation avant de les faire
 * référencées dans le tableau _animations du fichier \ref window.c
 *
 * Des squelettes d'animations et de transitions sont fournis pour
 * comprendre le fonctionnement de la bibliothèque. En bonus des
 * exemples dont un fondu en GLSL.
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date May 05, 2014
 */
#ifndef _ANIMATIONS_H

#define _ANIMATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

  extern void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void animation_flash(int state);
  extern void animation_vide(int state);
  extern void animation_damier(int state);
  extern void animation_base(int state);
  extern void animation_kids(int state);
  extern void animation_kids2(int state);
  extern void animationsInit(void);
  extern void animation_texte(int state);
  extern void animation_chenille(int state);
  extern void animation_thomas(int state);
  /* Dans base.c */
  extern void base_init(void);
  extern void base_draw(void);
  /* Dans kids.c */
  extern void kids_init(void);
  extern void kids_draw(void);
  /* Dans kids2.c */
  extern void kids2_init(void);
  extern void kids2_draw(void);
  /* Dans texte.c */
  extern void texte_init(void);
  extern void texte_draw(void);
  /* Dans chenille.c */
  extern void chenille_init(void);
  extern void chenille_draw(void);
  /* Dans thomas.c */
  extern void thomas_init(void);
  extern void thomas_draw(void);
#ifdef __cplusplus
}
#endif

#endif
