#ifndef CAR_H
#define CAR_H
 
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

#define SFONDOX 800
#define SFONDOY 600

// parametri da passare alla funzione per inizializzarla
struct parametri
{
    float inheight; // altezza del veicolo
    float inwidth; // larghezza del veicolo
    float inpx; // posizione x del veicolo
    float inpy; // posizione y del veicolo
    int indiam; // diametro dei cerchioni (16, 17, 18)
    int inass; // assetto della macchina (1, 2, 3)
};


// parametri carrozzeria
struct coca_carrozzeria
{
    float cx, cy;
    float width, height;
};

// parametri ruota
struct coca_ruota
{
    float ruota, cerchione;
    float centrox, centroy;
};

// parametri tetto
struct coca_tetto 
{
    float x1, y1; 
    float x2, y2;
    float x3, y3; 
    float x4, y4;
    float x5, y5;
};

// parametri finestrini
struct coca_finestrini
{
    float p1x, p1y; 
    float p2x, p2y;
    float p3x, p3y;
};

// parametri spoiler
struct coca_spoiler
{
    float px, py;
    float widths, heights;
};

// parametri dell'intero device
struct coca_device
{
    coca_carrozzeria car;
    coca_ruota sx;
    coca_ruota dx;
    coca_finestrini fin;
    coca_spoiler spoil;
    coca_tetto cap;
};

// intestazione + fine
std::string coca_intestazione();
std::string coca_sfondo();
std::string coca_fine();

// funzioni carrozzeria
void coca_try_carrozzeria(coca_device* macch);
std::string coca_strg_carrozzeria(coca_device* macch);

// funzioni ruote
void coca_try_ruote(coca_device* macch);
void coca_try_assetto(coca_device* macch);
std::string coca_strg_ruote(coca_device* macch);

// funzioni finestrini
void coca_try_finestrini(coca_device* macch);
std::string coca_strg_finestrini(coca_device* macch);

// funzioni spoiler
void coca_try_spoiler(coca_device* macch);
std::string coca_strg_spoiler(coca_device* macch);

// funzioni tetto
void coca_try_tetto(coca_device* macch);
std::string coca_strg_tetto(coca_device* macch);

// funzioni del device
void coca_try_device(coca_device* macch);
std::string coca_strg_device(coca_device* macch, int scelta);

// funzione scrive su file
void coca_write(string svg);

// funzione legge un file
string coca_read();

// funzione che inizzializza il device passando dei parametri
coca_device* coca_init_device(parametri par);

#endif //CAR_H