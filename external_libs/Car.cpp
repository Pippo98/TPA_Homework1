using namespace std;

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

#include "Car.h"
 
// errori
void coca_error(int err){
    
    switch (err)
    {
    case 0:
        cout << "ERROR: Scelta non permessa" << endl;
        break;
    case 1:
        cout << "ERROR: Posizione x non corretta, macchina fuori dal foglio di lavoro" << endl;
        break;
    case 2:
        cout << "ERROR: Posizione y non corretta, macchina fuori dal foglio di lavoro" << endl;
        break;
    case 3:
        cout << "ERROR: Cerchioni non disponibili" << endl;
        break;
    case 4:
        cout << "ERROR: Assetto non disponibile" << endl;
        break;
    case 5:
        cout << "ERROR: File non presente, devi prima scrivere o caricare un nuovo file per potermo salvare" << endl;
        break;
    default:
        break;
    }
    //cout << "GOING BACK TO MAIN MENU" << endl;
}

// My init parametri da funzione
coca_device* coca_init_device(parametri par){

    coca_device* indev = new coca_device;

    // carrozzeria
    indev->car.width = par.inwidth;
    indev->car.height = par.inheight;
    indev->car.cx = par.inpx;
    if((indev->car.cx + indev->car.width) > SFONDOX) coca_error(1);
    indev->car.cy = par.inpy;
    if((indev->car.cy + indev->car.height) > SFONDOY) coca_error(2);

    // ruote
    switch (par.indiam)
    {
        case 16:
            indev->sx.ruota = (indev->car.height - 1) / 2;
            indev->sx.cerchione = indev->sx.ruota / 1.6;
            indev->dx.ruota = (indev->car.height - 1) / 2;
            indev->dx.cerchione = indev->dx.ruota / 1.6;
            break;
        case 17:
            indev->sx.ruota = indev->car.height / 2;
            indev->sx.cerchione = indev->sx.ruota / 1.6;
            indev->dx.ruota = indev->car.height / 2;
            indev->dx.cerchione = indev->dx.ruota / 1.6;
            break;
        case 18:
            indev->sx.ruota = (indev->car.height + 1) / 2;
            indev->sx.cerchione = indev->sx.ruota / 1.6;
            indev->dx.ruota = (indev->car.height + 1) / 2;
            indev->dx.cerchione = indev->dx.ruota / 1.6;
            break;
        
        default:
            coca_error(3);

            break;
    }

    // assetto
    switch (par.inass){
        case 1:
            indev->sx.centrox = indev->car.cx + (indev->car.width / 5) - 5;
            indev->dx.centrox = indev->car.cx + indev->car.width - (indev->car.width / 5) + 5;
            indev->sx.centroy = indev->car.cy + indev->car.height;
            indev->dx.centroy = indev->car.cy + indev->car.height;
            break;
        case 2:
            indev->sx.centrox = indev->car.cx + (indev->car.width / 5);
            indev->dx.centrox = indev->car.cx + indev->car.width - (indev->car.width / 5);
            indev->sx.centroy = indev->car.cy + indev->car.height;
            indev->dx.centroy = indev->car.cy + indev->car.height;
            break;
        case 3:
            indev->sx.centrox = indev->car.cx + (indev->car.width / 5) + 5;
            indev->dx.centrox = indev->car.cx + indev->car.width - (indev->car.width / 5) - 5;
            indev->sx.centroy = indev->car.cy + indev->car.height;
            indev->dx.centroy = indev->car.cy + indev->car.height;
            break;

        default:
        coca_error(4);
            break;
        }

    // finestrini
    indev->fin.p1x = indev->car.cx + indev->car.width / 1.75;
    indev->fin.p1y = indev->car.cy - indev->car.height / 1.33333;

    indev->fin.p2x = indev->car.cx + indev->car.width / 1.25;
    indev->fin.p2y = indev->car.cy;

    indev->fin.p3x = indev->car.cx + indev->car.width / 1.75;
    indev->fin.p3y = indev->car.cy;

    // spoiler
    indev->spoil.heights = indev->car.height / (2.666667);
    indev->spoil.widths = indev->car.width / 17.5;
    indev->spoil.px = indev->car.cx + indev->car.width /35;
    indev->spoil.py = indev->car.cy - indev->spoil.heights;

    // tetto
    indev->cap.x1 = indev->car.cx + indev->car.width / 5.8333;
    indev->cap.y1 = indev->car.cy;

    indev->cap.x2 = indev->car.cx + indev->car.width / 3.5;
    indev->cap.y2 = indev->car.cy - indev->car.height / 1.33333;

    indev->cap.x3 = indev->car.cx + indev->car.width / 1.75;
    indev->cap.y3 = indev->car.cy - indev->car.height / 1.33333;

    indev->cap.x4 = indev->car.cx + indev->car.width / 1.75;
    indev->cap.y4 = indev->car.cy;

    indev->cap.x5 = indev->car.cx + indev->car.width / 5.8333;
    indev->cap.y5 = indev->car.cy;

    return indev;
}

// Stringa di Intestazione
string coca_intestazione(){
    string inte;
    inte = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\n";
    inte += "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
    inte += "width=\"" + to_string(SFONDOX) + "\" ";
    inte += "height=\"" + to_string(SFONDOY) + "\">\n\n";
    return inte;
}

string coca_sfondo(){
    string sfon;
    sfon = "\t<rect  x=\"0\" y=\"0\" ";
    sfon += "width=\"" + to_string(SFONDOX) + "\" ";
    sfon += "height=\"" + to_string(SFONDOY) + "\" ";
    sfon += "style=\"fill:rgb(255,255,255);stroke-width:3;stroke:rgb(0,0,0)\"/>\n\n"; 
    return sfon;
}

// Stringa finale
string coca_fine(){
    string fin;
    fin = "</svg>\n";
    return fin;
    
}

// Funzioni che implementano la carrozzeria
void coca_try_carrozzeria(coca_device* macch){
    int scelta;
    cout << "Inserire la lunghezza della macchina: ";
    cin >> macch->car.width;
    cout << "Inserire l'altezza della macchina: ";
    cin >> macch->car.height;

    cout << "Vuoi indicare la posizione della macchina nel foglio?" << endl;
    while(scelta != 1 && scelta != 2){
        cout << "[1] -> Si" << endl;
        cout << "[2] -> No" << endl;
        cin >> scelta;
        if(scelta != 1 && scelta != 2) coca_error(0);
    }
    if(scelta == 1){

        cout << "Indicare la la posizione x della macchina nel foglio" << endl;
  
        do
        {
            cin >> macch->car.cx;
            if((macch->car.cx + macch->car.width) > SFONDOX) coca_error(1);
        } while ((macch->car.cx + macch->car.width) > SFONDOX);

        cout << "Indicare la la posizione y della macchina nel foglio" << endl;

        do
        {
            cin >> macch->car.cy;
            if((macch->car.cy + macch->car.height) > SFONDOY) coca_error(2);
        } while ((macch->car.cy + macch->car.height) > SFONDOY);  
    } 
    
    if(scelta == 2) {
        macch->car.cx = (SFONDOX/2) - (macch->car.width/2);
        macch->car.cy = (SFONDOY/2) - (macch->car.height/2);
    }
}

string coca_strg_carrozzeria(coca_device* macch){
    string carr;
    carr = "\t<rect  ";
    carr += "x=\"" + to_string(macch->car.cx) + "\" ";
    carr += "y=\"" + to_string(macch->car.cy) + "\" ";
    carr += "rx=\"10\" ry=\"10\"";
    carr += " width=\"" + to_string(macch->car.width) + "\"";
    carr += " height=\"" + to_string(macch->car.height) + "\"";
    carr += " style=\"stroke-width:3;stroke:rgb(0,0,0)\" fill=\"red\"/>\n\n"; 
    return carr;
}

// Funzioni che implementano le ruote
void coca_try_ruote(coca_device* macch){
    
    int diametro;

    cout << "Scegliere il diametro dei cerchioni" << endl;
    while(diametro != 16 && diametro != 17 && diametro != 18){
        cout << "Cerchioni disponibili:" << endl;
        cout << "16 pollici\n17 pollici\n18 pollici\n" << endl;
        cin >> diametro;
            if(diametro != 16 && diametro != 17 && diametro != 18) coca_error(3);
    }

    switch (diametro)
    {
        case 16:
            macch->sx.ruota = (macch->car.height - 1) / 2;
            macch->sx.cerchione = macch->sx.ruota / 1.6;
            macch->dx.ruota = (macch->car.height - 1) / 2;
            macch->dx.cerchione = macch->dx.ruota / 1.6;
            break;
        case 17:
            macch->sx.ruota = macch->car.height / 2;
            macch->sx.cerchione = macch->sx.ruota / 1.6;
            macch->dx.ruota = macch->car.height / 2;
            macch->dx.cerchione = macch->dx.ruota / 1.6;
            break;
        case 18:
            macch->sx.ruota = (macch->car.height + 1) / 2;
            macch->sx.cerchione = macch->sx.ruota / 1.6;
            macch->dx.ruota = (macch->car.height + 1) / 2;
            macch->dx.cerchione = macch->dx.ruota / 1.6;
            break;
        
        default:
            break;
    }
}

void coca_try_assetto(coca_device* macch){
    int x;
    cout << "Sceglier l'assetto della macchina" << endl;
        while(x!= 1 && x!= 2 && x!= 3){
        cout << "Assetti disponibili:" << endl;
        cout << "Assetto pista = 1\nAssetto strada = 2\nAssetto fuoristrada = 3\n" << endl;
        cin >> x;
            if(x!= 1 && x!= 2 && x!= 3) coca_error(4);
        }
    switch (x){
        case 1:
            macch->sx.centrox = macch->car.cx + (macch->car.width / 5) - 5;
            macch->dx.centrox = macch->car.cx + macch->car.width - (macch->car.width / 5) + 5;
            macch->sx.centroy = macch->car.cy + macch->car.height;
            macch->dx.centroy = macch->car.cy + macch->car.height;
            break;
        case 2:
            macch->sx.centrox = macch->car.cx + (macch->car.width / 5);
            macch->dx.centrox = macch->car.cx + macch->car.width - (macch->car.width / 5);
            macch->sx.centroy = macch->car.cy + macch->car.height;
            macch->dx.centroy = macch->car.cy + macch->car.height;
            break;
        case 3:
            macch->sx.centrox = macch->car.cx + (macch->car.width / 5) +5;
            macch->dx.centrox = macch->car.cx + macch->car.width - (macch->car.width / 5) - 5;
            macch->sx.centroy = macch->car.cy + macch->car.height;
            macch->dx.centroy = macch->car.cy + macch->car.height;
            break;

        default:
            break;
        }
}

string coca_strg_ruote(coca_device* macch){
    string ruo;
    // Ruota sinistra
    ruo = "\t<circle ";
    ruo += "cx=\"" + to_string(macch->sx.centrox) + "\" ";
    ruo += "cy=\"" + to_string(macch->sx.centroy) + "\" ";
    ruo += "r=\"" + to_string(macch->sx.ruota) + "\" stroke=\"black\" stroke-width=\"3\" fill=\"black\"/>\n\n";
    // Cerchione sinistra
    ruo += "\t<circle ";
    ruo += "cx=\"" + to_string(macch->sx.centrox) + "\" ";
    ruo += "cy=\"" + to_string(macch->sx.centroy) + "\" ";
    ruo += "r=\"" + to_string(macch->sx.cerchione) + "\" stroke=\"black\" stroke-width=\"3\" fill=\"gray\"/>\n\n";
    // Ruota destra
    ruo += "\t<circle ";
    ruo += "cx=\"" + to_string(macch->dx.centrox) + "\" ";
    ruo += "cy=\"" + to_string(macch->dx.centroy) + "\" ";
    ruo += "r=\"" + to_string(macch->dx.ruota) + "\" stroke=\"black\" stroke-width=\"3\" fill=\"black\"/>\n\n";
    // Cerchione destra
    ruo += "\t<circle ";
    ruo += "cx=\"" + to_string(macch->dx.centrox) + "\" ";
    ruo += "cy=\"" + to_string(macch->dx.centroy) + "\" ";
    ruo += "r=\"" + to_string(macch->dx.cerchione) + "\" stroke=\"black\" stroke-width=\"3\" fill=\"gray\"/>\n\n";

    return ruo;
}

// Funzione che implementa i finestrini
void coca_try_finestrini(coca_device* macch){

    macch->fin.p1x = macch->car.cx + macch->car.width / 1.75;
    macch->fin.p1y = macch->car.cy - macch->car.height / 1.33333;

    macch->fin.p2x = macch->car.cx + macch->car.width / 1.25;
    macch->fin.p2y = macch->car.cy;

    macch->fin.p3x = macch->car.cx + macch->car.width / 1.75;
    macch->fin.p3y = macch->car.cy;
}

string coca_strg_finestrini(coca_device* macch){
    string fines;
    fines = "\t<polygon points=\"";
    fines += to_string(macch->fin.p1x) + "," + to_string(macch->fin.p1y) + " ";
    fines += to_string(macch->fin.p2x) + "," + to_string(macch->fin.p2y) + " ";
    fines += to_string(macch->fin.p3x) + "," + to_string(macch->fin.p3y) + "\" ";
    fines += "style=\"fill:lightblue;stroke:black;stroke-width:5\" />\n\n";
    return fines;
}

// Funzione che implementa lo spoiler
void coca_try_spoiler(coca_device* macch){
    macch->spoil.heights = macch->car.height / (2.666667);
    macch->spoil.widths = macch->car.width / 17.5;
    macch->spoil.px = macch->car.cx + macch->car.width /35;
    macch->spoil.py = macch->car.cy - macch->spoil.heights;
}

string coca_strg_spoiler(coca_device* macch){
    string spoil;
    spoil = "\t<rect ";
    spoil += "x=\"" + to_string(macch->spoil.px) + "\" ";
    spoil += "y=\"" + to_string(macch->spoil.py) + "\" "; 
    spoil += "width=\"" + to_string(macch->spoil.widths) + "\" ";
    spoil += "height=\"" + to_string(macch->spoil.heights) + "\" "; 
    spoil += "style=\"stroke-width:3;stroke:rgb(0,0,0)\" fill=\"red\"/>\n\n";
    return spoil;
}

// Funzione che implementa il tetto
void coca_try_tetto(coca_device* macch){
    macch->cap.x1 = macch->car.cx + macch->car.width / 5.8333;
    macch->cap.y1 = macch->car.cy;

    macch->cap.x2 = macch->car.cx + macch->car.width / 3.5;
    macch->cap.y2 = macch->car.cy - macch->car.height / 1.33333;

    macch->cap.x3 = macch->car.cx + macch->car.width / 1.75;
    macch->cap.y3 = macch->car.cy - macch->car.height / 1.33333;

    macch->cap.x4 = macch->car.cx + macch->car.width / 1.75;
    macch->cap.y4 = macch->car.cy;

    macch->cap.x5 = macch->car.cx + macch->car.width / 5.8333;
    macch->cap.y5 = macch->car.cy;

}

string coca_strg_tetto(coca_device* macch){
    string te;
    te = "\t<polygon points=\"";
    te += to_string(macch->cap.x1) + "," + to_string(macch->cap.y1) + " ";
    te += to_string(macch->cap.x2) + "," + to_string(macch->cap.y2) + " ";
    te += to_string(macch->cap.x3) + "," + to_string(macch->cap.y3) + " ";
    te += to_string(macch->cap.x4) + "," + to_string(macch->cap.y4) + " ";
    te += to_string(macch->cap.x5) + "," + to_string(macch->cap.y5) + "\" ";
    te += "style=\"fill:red;stroke:black;stroke-width:5\"/>\n\n";
    return te;
}

// Funzione che implementa il device
void coca_try_device(coca_device* macch){
    coca_try_carrozzeria(macch);
    coca_try_ruote(macch);
    coca_try_assetto(macch);
    coca_try_finestrini(macch);
    coca_try_spoiler(macch);
    coca_try_tetto(macch);
}

string coca_strg_device(coca_device* macch, int scelta){
    string svg;
    if(scelta == 1){
        svg = coca_intestazione();
        svg += coca_sfondo();
    }
    svg += coca_strg_carrozzeria(macch);
    svg += coca_strg_ruote(macch);
    svg += coca_strg_tetto(macch);
    svg += coca_strg_finestrini(macch);
    svg += coca_strg_spoiler(macch);
    if(scelta == 1){
        svg += coca_fine();
    }
    return svg;
}

// Funzione che scrive su file
void coca_write(string svg){

    if(svg == "") coca_error(5);
    if(svg != ""){
        string nome;

        cout << "Scrivere il nome del file (es: nomefile.svg)" << endl;
        cin >> nome;

        // Create and open a text file
        ofstream MyFile(nome);

        string string_to_write = svg;

        // Write to the file
        MyFile << string_to_write;

        // Close the file
        MyFile.close();
    }
}

// Funzione che legge da file
string coca_read(){

    string file, lettura;

    cout << "Scrivere il nome del file (es: nomefile.svg)" << endl;
    cin >> lettura;

    std::ifstream t(lettura);
    std::stringstream buffer;
    buffer << t.rdbuf();
    file = buffer.str();

    return file;
}