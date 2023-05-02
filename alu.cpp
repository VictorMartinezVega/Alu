#include "alu.h"

Alu::Alu(float n1, float n2)
{
this->num1.decimal = n1;
this->num2.decimal = n2;

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
        //Normalizamos la mantisa con mantisa1|8388608 o con la mantisa que estemos utilizando
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
    //paso7
    if(num1.bits.signo != num2.bits.signo){
        for(int i = 0; i < d; i++){
            p = p>>1;
            p = p|8388608;
        }
    }else{
        p = p>>d;
    }

    //paso8
    p = mantisa1 + p;
    int resto = ((p>>n)&1);

    //paso9
    if(d == 0){
        if((num1.bits.signo != num2.bits.signo) && ((p>>(n-1))&1) && (resto == 0)){
            p = this->comp2(p);
            complementado_p = true;
        }
    }

    //paso104
    int k = 0;
    unsigned int aux = p;

    if((num1.bits.signo == num2.bits.signo) && (resto == 1)){
        st = g|r|st;
        r = ((p>>0)&1);
        p = p>>1;
        exponenteSuma = exponenteSuma + 1;
    }else{
        while(((aux>>(n-1))&1) == 0){
            aux = aux << 1;
            k++;
        }

        if(k == 0){
            st = r|st;
            r = g;
        }else if(k > 1){
            r = 0;
            st = 0;
        }

        unsigned int x = 16777215;

        x = x >> k;
        p = p&x;

        if(g == 1){
            p = p|1;
        }else{
            p = p<<k;
        }

        exponenteSuma = exponenteSuma - k;

    }

    //paso11
    if((r == 1 && st == 1) || (r == 1 && st == 0 && ((p>>0)&1))){
        p = p + 1;
        int acarreo2 = ((p>>n)&1);

        if(acarreo2 == 1){
            p = p>>1;
            exponenteSuma = exponenteSuma + 1;
        }
    }

    mantisaSuma = p;

    //paso12
    if(intercambio == false && complementado_p == true ){
        signoSuma = num2.bits.signo;
    }else{
        signoSuma = num1.bits.signo;
    }

    //paso13
    resultado.bits.signo = signoSuma;
    resultado.bits.exponente = signoSuma;
    resultado.bits.mantisa = mantisaSuma;

}


unsigned int Alu:: comp2(unsigned int mantisaB){
    mantisaB = ~mantisaB + 1;
    mantisaB = mantisaB^4278190080;

    return mantisaB;
}










