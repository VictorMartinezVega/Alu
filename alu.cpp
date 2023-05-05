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
    unsigned int as;
    unsigned int ps;
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

void Alu::producto(){
    unsigned int signoProd;
    unsigned int exponenteProd;
    unsigned int mantisaProd;
    unsigned int mantisa1 = this->num1.bits.mantisa;
    unsigned int mantisa2 = this->num2.bits.mantisa;

    //paso1
    signoProd = this->num1.bits.signo ^ this->num2.bits.signo;

    //paso2
    exponenteProd = (this->num1.bits.exponente - 127) + (this->num2.bits.exponente - 127) + 127;

        //anyadimos en uno al principio de la mantisa
    mantisa1 = mantisa1|8388608;
    mantisa2 = mantisa2|8388608;

    //paso3

        //i - algoritmo producto sin signo
    this->algoritmoProductoSinSigno(mantisa1, mantisa2);

        //ii - se comprueba si desplazar P un bit a la izq o sumar 1 al exponente del producto
    if(!((ps>>23)&1)){
        ps = ps << 1;
    }else{
        exponenteProd = exponenteProd +1;
    }

        //iii - Bit de redondeo
    int r = ((as>>23)&1);

        //iv - Bit sticky
    int st = 0;
    for(int i = 22; i >= 0; i--){
        st = (st|(as>>(1))&1);
    }

        //v - Redondeo
    if((r == 1 && st == 1) || (r == 1 && st == 0 && (ps>>0)&1 == 1)){
        ps = ps + 1;
    }

        //COMPROBACION DE DESBORDAMIENTO
        //Desbordamiento a infinito
    if (exponenteProd >= 255){
        this->overFlow = true;
        return;
    }

        //desbordamiento a cero
    if(exponenteProd < 1){
        int t = 1 - exponenteProd;

        if(!(t >= 24)){
            //desplazar P y A t bits aritmétricamente
            for(int i = 0; i < t; i++){
                ps = ps >> 1;
                if((ps >> 23)&1){
                    ps = ps|8388608;
                }
            }
            exponenteProd = 1;
        }else{
            this->underFlow = true;
        }
    }

    //OPERANDOS DENORMALES
    if(this->num1.bits.exponente == 0 || this->num2.bits.exponente == 0){
        //caso1
        if(exponenteProd < 1){
            int t = 1 - exponenteProd;
            if(!t >= 24){
                //desplazar P y A t bits aritméticamente
                for(int i = 0; i < t; i++){
                    ps = ps >> 1;
                    if((ps>>23)&1){
                        ps = ps|8388608;
                    }
                }
                exponenteProd = 1;
            }
            //caso2
        }else if(exponenteProd > 1){
            int t1 = exponenteProd - 1;
            int t2;
            unsigned int aux = ps;

            while(!((aux>>0)&1)){
                aux = aux >> 1;
                t2 = t2 + 1;
            }
            int t;
            if(t1 > t2){
                t = t2;
            }else{
                t = t1;
            }
            exponenteProd = exponenteProd - t;

            for(int i = 0; i < t; i++){
                ps = ps << 1;
                if((ps>>23)&1){
                    as = as>>1;
                    as = as|8388608;
                }
            }
            //caso3
        }else{

        }

    }
}

void Alu::algoritmoProductoSinSigno(unsigned int A, unsigned int B) {
    as = A;
    ps = 0;
    //Se repite n veces
    for(int i = 0; i < 24; i++){
        if(((as>>(0))&1)){
            ps = ps + B;
        }
        ps = ps>>1;
        as = as>>1;
    }
}

void Alu::division(){
    unsigned int ed, md, sd, e1, e2;

    union operando aux1 = this->num1;
    union operando aux2 = this->num2;

    sd = this->num1.bits.signo^this->num2.bits.signo;
    e1 = this->num1.bits.exponente;
    e2 = this->num2.bits.exponente;

    //paso1 ESCALAR A Y B PARA QUE B PERTENEZCA A [1,2)
    float man1 = this->num1.bits.mantisa;
    float num = 8388608;
    float aR = 1 + man1/num;
    float man2 = this->num2.bits.mantisa;
    float bR = 1+man2/num;

    //Paso2 Buscar en la tabla una solucion aproximada b
    float b;
    if(bR<1.25){
        b = 1.00;
    }else{
        b = 0.80;
    }

    //Paso3
    float X0 = productoP(aR, b);
    float Y0 = productoP(bR, b);

    //Paso 4 Se itera hasta que el valor entre un resultado y el de la operacion anterior sea menor que 10^4
    bool primer = true;
    float Xi = X0;
    float Xa = 0;
    float Yi = Y0;
    float r = 0;

    while(((abs(Xi-Xa))> pow(10, -4)) || (primer)){
        primer = false;
        Xa = Xi;
        r = sumaP(2, -Yi);
        Yi = productoP(Yi,r);
        Xi = productoP(Xi, r);
    }

    //Pasar Xi a binario
    union operando xi;
    xi.decimal = Xi;
    md = xi.bits.exponente;
    ed = e1 - e2 + xi.bits.exponente;

    this->resultado.bits.signo = sd;
    this->resultado .bits.exponente = ed;
    this->resultado.bits.mantisa = md;
}

float Alu::sumaP(float a, float b){
    this->num1.decimal = a;
    this->num2.decimal = b;
    this->producto();

    return resultado.decimal;

}

float Alu::productoP(float a, float b){
    this->num1.decimal = a;
    this->num2.decimal = b;
    this->producto();

    return resultado.decimal;
}

unsigned int Alu:: comp2(unsigned int mantisaB){
    mantisaB = ~mantisaB + 1;
    mantisaB = mantisaB^4278190080;

    return mantisaB;
}










