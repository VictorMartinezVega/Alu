#ifndef ALU_H
#define ALU_H


class Alu
{
public:
    Alu(float n1, float n2);
    void suma();
    unsigned int comp2(unsigned int mantisaB);
private:
    int n;
    int g;
    int st;
    int r;
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
