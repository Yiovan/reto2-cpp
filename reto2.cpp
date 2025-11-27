#include<iostream>

using namespace std;

void mapa();

// void personajes(){
//     string personaje;
//     cout<<'ingrese tu personaje'<<endl;
//     cin>>personaje;
//     if (personaje == 'r'){
//         cout<<"tu personaje es una rata"<<endl;
        
//     }else if (personaje == 'g'){
//         cout<<'tu personaje es un gato'<<endl;
//     }

}
int main(){
    //personajes();
     mapa();
}


void mapa(){
    int ancho = 5; 
    int largo = 5; 
    
    for (int i = 0; i < largo; i++) { 
    
        for (int e = 0; e < ancho; e++) {
            
            if (i == 0 || i == largo - 1 || e == 0 || e == ancho - 1) {
                cout << "* ";
            } else {
                cout << "  "; 
            }
        }
        
        cout << endl; 
    }

 

}