#ifndef ALU_H
#define ALU_H
#include "math.h"


class Alu
{
public:
    Alu(float n1, float n2);
    void suma();
    void producto();
    void algoritmoProductoSinSigno(unsigned int A, unsigned int B);
    void division();
    float sumaP(float a, float b);
    float productoP(float a, float b);
    unsigned int comp2(unsigned int mantisaB);
private:
    bool desbordamiento = false;
    bool overFlow = false;;
    bool underFlow = false;
    int n;
    int g;
    int st;
    int r;
    int as;
    int ps;
    struct binario{
        unsigned int mantisa :23;
        unsigned int signo:1;
        unsigned int exponente:8;

    };

    union operando{
        float decimal;
        unsigned int pEntera;
        struct binario bits;
    };
    union operando num1;
    union operando num2;
    union operando resultado;


};

#endif // ALU_H
