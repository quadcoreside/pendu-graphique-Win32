#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

void strToLower(char str[]);
void my_strip(char *data);
int random(int min, int max);
void testMot();
void confPartieSolo();
void confPartieADeux();
void recommencerPartie();
LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

#define BTN_RECOMMENCER 100
#define BTN_VALIDER 102
#define BTN_JOUER_SOLO 103
#define BTN_JOUER_A_DEUX 104
#define BTN_QUITTER 105

#define EDIT_MOT_SECRET 106
#define EDIT_PHRASE_INDICE 107
#define EDIT_MOT_ENTRER 108

#define RDI_NIV1 109
#define RDI_NIV2 110
#define RDI_NIV3 111

#define MAX_LENGHT_MOT 5
#define MAX_LENGHT_PHRASE_INDICE 100

#endif // MAIN_H_INCLUDED
