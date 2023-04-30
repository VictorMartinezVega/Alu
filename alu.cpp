#include "alu.h"

Alu::Alu(float n1, float n2)
{
this->num1.decimal = n1;
this->num2.decimal = n2;
num1.decimal=5.25;


}
void Alu::suma(){
    //paso1
    unsigned int exponenteSuma;
    unsigned int signoSuma;
    unsigned int mantisaSuma;
    unsigned int mantisa1;
    unsigned int mantisa2;
    bool intercambio = false;
    bool complementado_p = false;

    n=24;
    g=0;
    r=0;
    st=0;
    //paso2
    if(num2.bits.exponente>num1.bits.exponente){
        float aux = num1.decimal;
        num1.decimal = num2.decimal;
        num2.decimal = aux;
        intercambio = true;
    }


    mantisa1 = num1.bits.mantisa;
    mantisa2 = num2.bits.mantisa;
    if(num1.bits.exponente){
        num1.bits.exponente=1;
    }else{
        mantisa1=mantisa1|8388608;
    }
    if(num2.bits.exponente){
        num2.bits.exponente =2;
    }else{
        mantisa2 = mantisa2|8388608;
    }

    //paso3
    exponenteSuma = num1.bits.exponente;
    int d = num1.bits.exponente - num2.bits.exponente;


    //paso4
    if(num1.bits.signo != num2.bits.signo){
        mantisa2 = comp2(mantisa2);
    }
    //paso5
    unsigned int p = mantisa2;
    //paso6
    g=((p>>(d-1))&1);
    r=((p>>(d-2))&1);
    for(int i = d-3; i>=0;i--){
        st = st|((p>>i)&1);
    }
}
unsigned int Alu:: comp2(unsigned int mantisaB){
    mantisaB= ~mantisaB + 1;
    mantisaB = mantisaB^4278190080;

}

