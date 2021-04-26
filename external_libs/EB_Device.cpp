
#include "EB_Device.h"
#include <iostream>
#include <string.h>
#include <cmath>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>

using namespace std;

/*
    some components' features are fixed --> shaft width, towtruck height, platform height 
*/
    const double std_shaftWidth = 20;
    const double std_towtruckHeight = 40;
    const double std_platformHeight = 20;

    const double std_Ycir=120;
    const double std_radius=10;
    const double std_YtowTruck=100;
/*
    centro di istantanea rotazione --> punto attorno al quale l'asta ruota, corrisponde al
    centro della coppia rotoidale che unisce asta e carrello --> Ycir è fissa, Xcir varia in
    funzione dello spostamento del carrello lungo x
*/


/**
    A function initing a device 
**/
EbDevice* eb_init(double length_shaft, double width_towtruck, double width_platform, double rotation, double sliding){
    
    EbDevice* eb_device = new EbDevice;

    if(eb_checkConstraints(length_shaft,width_towtruck,width_platform,rotation,sliding)==false){
        return NULL;
    }
    else{
        eb_device -> length_shaft = length_shaft;
        eb_device -> width_towtruck = width_towtruck;
        eb_device ->  width_platform = width_platform;
        eb_device ->  rotation = rotation;  //angolo di rotazione è rispetto all'asse y, rotazione in senso orario se angolo positivo
        eb_device ->  sliding = sliding;
    }

    return eb_device;
}

/**
    A function checking mechanical constraints  
**/
bool eb_checkConstraints(double length_shaft, double width_towtruck, double width_platform, double rotation, double sliding){
    
    if(length_shaft < 0 || width_towtruck < 0 || width_platform < 0 || sliding < 0){
        return false;
    }
    if(length_shaft<((std_platformHeight/2)+(std_towtruckHeight/2))){   
        return false;
    }
    if(rotation > 80 || rotation < -80){      //angoli max = +- 80 --> limite "fisico" max
        return false;
    }
    if(width_towtruck < std_shaftWidth){
        return false;
    }

    if(width_platform < std_shaftWidth){
        return false;
    }
    return true;
} 

/**
    A function checking costraints in relation to the svg draw
**/
bool eb_drawConstraints(EbDevice* eb_device){

    if(eb_Yplatform(eb_device) > 980){   //vincolo in altezza
        return false;
    }
    if((eb_Xcir(eb_device) + (eb_device->width_towtruck/2)) > 1500 || (eb_Xplatform(eb_device)+(eb_device->width_platform/2)) > 1500){ //vincolo in larghezza(a dx)
        return false;
    }
    if((eb_Xplatform(eb_device) - ((eb_device->width_platform)/2)) < 0){ //vincolo in larghezza(a sx)
        return false;
    }      

    return true;
}

/**
    A function which produce a string with svg code
    if with_measures is true the drawing will include measurements   
**/
string eb_to_svg(EbDevice* eb_device, bool with_measures){

    if(eb_drawConstraints(eb_device)==false){
        return "";
    }
    
    /*parametri device*/
    double sliding=eb_device->sliding;
    double length=eb_device->length_shaft;
    double angle=eb_device->rotation; 
    double widthPla=eb_device->width_platform;
    double widthTt=eb_device->width_towtruck;

    /*variabili utili*/
    double Xcir=eb_Xcir(eb_device);
    double Xplatform=eb_Xplatform(eb_device);
    double Yplatform=eb_Yplatform(eb_device);

    string code="";

    if(with_measures==true){
        /*
            disegno con misure
        */
        code+="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
        code+="<svg xmlns=\"http://www.w3.org/2000/svg\" style=\"background-color:white\" width=\"1500\" height=\"1000\">\n\n";
        
        /*
            carrello gru con spostamento orizzontale
        */
        code+="<g>\n";
        code+="<rect x = \""+to_string(sliding)+"\" y = \"100\" width = \"" + to_string(widthTt) +"\" height = \"40\" stroke = \"black\" stroke-width = \"3\" fill = \"yellow\"/>\n";
        code+="<text x = \""+to_string(sliding)+"\" y = \"65\" fill = \"black\"> width_towtruck = "+to_string((int)sliding)+" </text>\n";
        code+="<line x1 = \""+to_string(sliding)+"\" y1 = \"75\" x2 = \""+to_string(sliding+widthTt)+"\" y2 = \"75\"  stroke = \"black\" stroke-width  = \"2\"/>\n";

        code+="</g>\n\n";

        /*
            asta rotante, 
            angolo positivo --> asta ruota in senso orario(verso sx)
            angolo negativo --> asta ruota in senso antiorario(verso dx)
        */
        code+="<g transform  = \"rotate("+to_string(angle)+","+to_string(Xcir)+",120)\">\n";
        code+="<rect x = \""+to_string(Xcir-std_radius)+"\" y = \"120\" width = \"20\" height = \"" + to_string(length) + "\" stroke = \"black\" stroke-width = \"3\" fill = \"orange\" />\n";
        code+="<text x = \""+to_string(Xcir+(widthTt/2)+40)+"\" y = \""+to_string(std_Ycir+(length/2))+"\" fill = \"black\"> length_shaft = "+to_string((int)length)+" </text>\n";
        code+="<line x1 = \""+to_string(Xcir+(widthTt/2)+30)+"\" y1 = \"120\" x2 = \""+to_string((Xcir+(widthTt/2)+30))+"\" y2 = \""+to_string((120+length))+"\"  stroke = \"black\" stroke-width  = \"2\"/>\n";
        code+="<text x = \""+to_string(Xcir-std_radius-80)+"\" y = \""+to_string(std_Ycir-10+(length/2))+"\" fill = \"black\"> angle="+to_string((int)angle)+" </text>\n";
        code+="<line x1 = \""+to_string(Xcir-std_radius-70)+"\" y1 = \""+to_string(std_Ycir+(length/2))+"\" x2 = \""+to_string(Xcir-std_radius)+"\" y2 = \""+to_string(std_Ycir+(length/2))+"\"  stroke = \"black\" stroke-width  = \"2\"/>\n";
        code+="<circle cx = \""+to_string(Xcir)+"\" cy = \"120\" r = \"10\" stroke = \"black\" stroke-width = \"3\" fill = \"white\"/>\n";
        code+="</g>\n\n";

        /*
            piattaforma
        */
        code+="<g>\n";
        code+="<rect x = \""+to_string(Xplatform)+"\" y = \""+to_string(Yplatform)+"\" width = \""+to_string(widthPla)+"\" height = \"20\"  stroke = \"black\" stroke-width = \"3\" fill = \"black\" />\n";
        code+="<text x = \""+to_string(Xplatform+(widthPla/4))+"\" y = \""+to_string(Yplatform+std_platformHeight+40)+"\" fill = \"black\"> width_platform = "+to_string((int)widthPla)+" </text>\n";
        code+="<line x1 = \""+to_string(Xplatform)+"\" y1 = \""+to_string(Yplatform+std_platformHeight+20)+"\" x2 = \""+to_string(Xplatform+widthPla)+"\" y2 = \""+to_string(Yplatform+std_platformHeight+20)+"\"  stroke = \"black\" stroke-width  = \"2\"/>\n";
        code+="</g>\n\n";

        /*
            quota spostamento carrello
        */    
        code+="<g>\n";
        if(sliding>100){
            code+="<text x = \""+to_string(sliding-100)+"\" y = \"110\" fill = \"black\"> sliding = "+to_string((int)sliding)+" </text>\n";
            code+="<line x1 = \""+to_string(sliding-80)+"\" y1 = \"120\" x2 = \""+to_string(sliding)+"\" y2 = \"120\"  stroke = \"black\" stroke-width  = \"2\"/>\n";
        }
        else{
            code+="<text x = \"10\" y = \"20\" fill = \"black\"> sliding = "+to_string((int)sliding)+" </text>\n";
            code+="<line x1 = \"2\" y1 = \"30\" x2 = \""+to_string(sliding+20)+"\" y2 = \"30\"  stroke = \"black\" stroke-width  = \"2\"/>\n";
        }
        code+="</g>\n";

        code+="</svg>\n";

        return code;
    }


    code+="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    code+="<svg xmlns=\"http://www.w3.org/2000/svg\" style=\"background-color:white\" width=\"1500\" height=\"1000\">\n\n";
    
    /*
        carrello gru con spostamento orizzontale
    */
    code+="<g>\n";
    code+="<rect x = \""+to_string(sliding)+"\" y = \"100\" width = \"" + to_string(widthTt) +"\" height = \"40\" stroke = \"black\" stroke-width = \"3\" fill = \"yellow\"/>\n";
    code+="</g>\n\n";

    /*
        asta rotante, 
        angolo positivo --> asta ruota in senso orario(verso sx)
        angolo negativo --> asta ruota in senso antiorario(verso dx)
    */
    code+="<g transform  = \"rotate("+to_string(angle)+","+to_string(Xcir)+",120)\">\n";
    code+="<rect x = \""+to_string(Xcir-std_radius)+"\" y = \"120\" width = \"20\" height = \"" + to_string(length) + "\" stroke = \"black\" stroke-width = \"3\" fill = \"orange\" />\n";
    code+="<circle cx = \""+to_string(Xcir)+"\" cy = \"120\" r = \"10\" stroke = \"black\" stroke-width = \"3\" fill = \"white\"/>\n";
    code+="</g>\n\n";

    /*
        piattaforma
    */
    code+="<g>\n";
    code+="<rect x = \""+to_string(Xplatform)+"\" y = \""+to_string(Yplatform)+"\" width = \""+to_string(widthPla)+"\" height = \"20\"  stroke = \"black\" stroke-width = \"3\" fill = \"black\" />\n";
    code+="</g>\n\n";

    code+="</svg>\n";

    return code;
}

/**
    A function which calculate coordinate X of CiR
**/
double eb_Xcir(EbDevice* eb_device){
    double Xcir;

    Xcir=eb_device->sliding + ((eb_device->width_towtruck)/2);

    return Xcir;
}

/**
    A function which calculate coordinate X of the platform
**/
double eb_Xplatform(EbDevice* eb_device){
    double Xplatform;
    double Xcir=eb_Xcir(eb_device);
    double angle;
    double l = eb_device->length_shaft;

    angle=eb_device->rotation * (M_PI/180);  //necessario angolo in radianti nella funzione seno

    Xplatform=Xcir - l*sin(angle) - ((eb_device->width_platform)/2);

    return Xplatform;
}

/**
    A function which calculate coordinate Y of the platform
**/
double eb_Yplatform(EbDevice* eb_device){
    double Yplatform;
    double angle;
    double l = eb_device->length_shaft;

    angle=eb_device->rotation * (M_PI/180);  //necessario angolo in radianti nella funzione seno

    Yplatform=std_Ycir + l*cos(angle) - (std_platformHeight/2);

    return Yplatform;
}

/*
    Sets a new length_shaft in the structure       
    if the new length is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_lengthShaft(EbDevice* device, double new_length_shaft){ 

    if(eb_checkConstraints(new_length_shaft,device->width_towtruck,device->width_platform,device->rotation, device->sliding) == false){
        return 1;
    }

    device->length_shaft = new_length_shaft;

    return 0;
}

/*
    Sets a new width_towtruck in the structure       
    if the new width is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_widthTowtruck(EbDevice* device, double new_width_towtruck){ 

    if(eb_checkConstraints(device->length_shaft,new_width_towtruck,device->width_platform,device->rotation, device->sliding) == false){
        return 1;
    }

    device->width_towtruck = new_width_towtruck;

    return 0;
}

/*
    Sets a new width_platform in the structure       
    if the new width is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_widthPlatform(EbDevice* device, double new_width_platform){ 

    if(eb_checkConstraints(device->length_shaft,device->width_towtruck,new_width_platform,device->rotation, device->sliding) == false){
        return 1;
    }

    device->width_platform = new_width_platform;

    return 0;
}

/*
    Sets  rotation in the structure       
    if the new rotation is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_rotation(EbDevice* device, double new_rotation){ 

    if(eb_checkConstraints(device->length_shaft,device->width_towtruck,device->width_platform,new_rotation, device->sliding) == false){
        return 1;
    }

    device->rotation = new_rotation;

    return 0;
}

/*
    Sets  sliding in the structure       
    if the new sliding is incompabile with other measures, RETURN 1, otherwise 0
*/
int eb_set_sliding(EbDevice* device, double new_sliding){ 

    if(eb_checkConstraints(device->length_shaft,device->width_towtruck,device->width_platform, device->rotation, new_sliding) == false){
        return 1;
    }

    device->sliding = new_sliding;

    return 0;
}

/*
    A function which write on a file svg
*/
void eb_save_to_file(string stringToWrite, string filename){
   
    // Create and open a text file
    ofstream MyFile(filename+".svg");

    // Write to the file
    MyFile << stringToWrite;

    // Close the file
    MyFile.close();
}

/*
    function, which creates a struct from a SVG textual representation
*/
EbDevice* eb_parse(string svg){
    
    double length_shaft;
    double width_towtruck;
    double width_platform;
    double rotation;
    double sliding;
    
    
    //getting sliding
    string search = "rect x = \"";
    size_t find1 = svg.find(search) + search.size();
    size_t find2= svg.find("\"", find1);
    string element = svg.substr(find1, find2-find1);
    sliding = stod(element);


    //getting width_towtruck
    search = "y = \"100\" width = \"";
    size_t find3 = svg.find(search,find1) + search.size();
    size_t find4= svg.find("\"", find3);
    element = svg.substr(find3, find4-find3);
    width_towtruck = stod(element);

    
    //getting rotation
    search = "g transform  = \"rotate(";
    size_t find5 = svg.find(search,find3) + search.size();
    size_t find6= svg.find(",", find5);
    element = svg.substr(find5, find6-find5);
    rotation = stod(element);
    
    //getting length_shaft
    search = "y = \"120\" width = \"20\" height = \"";
    size_t find7 = svg.find(search,find5) + search.size();
    size_t find8= svg.find("\"", find7);
    element = svg.substr(find7, find8-find7);
    length_shaft = stod(element);

    //getting width_platform
    search = " width = \"";
    size_t find9 = svg.find(search,find7) + search.size();
    size_t find10= svg.find("\"", find9);
    element = svg.substr(find9, find10-find9);
    width_platform = stod(element);


    EbDevice* device = new EbDevice;

    if(eb_checkConstraints(length_shaft,width_towtruck,width_platform,rotation,sliding) == false){
        return NULL;
    }
    
    device=eb_init(length_shaft,width_towtruck,width_platform,rotation,sliding);
    
    return device;
}


/*
    A function which read from a file a string
*/
string eb_read_from_file(string filename){
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    string svg_string = buffer.str();

    return svg_string;
}
