#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class Inversion {
    protected:
    double capital;
    int tiempo;
    public:
    //metodo cosntructor
    Inversion(double _capital, int _tiempo): capital(_capital), tiempo(_tiempo) {
    }

    //metodo destructor que se reutiliza en todas las subclases
    virtual ~Inversion() {
    }

    //setters y getters
    double getCapital() const {
        return capital;
    }
    int getTiempo() const {
        return tiempo;
    }
    void setCapital(double c) {
        capital = c;
    }
    void setTiempo(int t) {
        tiempo = t;
    }

    //metodos polimorficos
    virtual double rendimiento() const {
        return capital;
    }
    virtual void mostrar() const {
        cout << "Capital invertido: $" << capital << " Tiempo: " << tiempo << " anos\n" << endl;
    }
};

//primera subclase
class CETES : public Inversion {
    protected:
    double tasaCETES;

    public:
    CETES(double _capital, int _tiempo, double _tasaCETES) : Inversion(_capital, _tiempo), tasaCETES(_tasaCETES){
    }
    ~CETES() override {
    }

    //metodo polimorfico que calcula el rendimiento en CETES
    double rendimiento() const override {
        //recibe la tasa en numeros naturales como 7.4%
        double tasa = (tasaCETES / 100.0);
        //capital inicial multiplicado por 100% más el porcentaje de la tasa por la cantidad de años que han transcurrido
        double rend = capital *(1+tasa)*tiempo;
        return rend;
    }

    //metodo polimorfico para mostrar el rendimiento en CETES
    void mostrar() const override {
        cout << "CETES" << endl;
        Inversion::mostrar();
        cout << "Rendimiento: $" << rendimiento() << "\n" << endl;
    }
};

//segunda sublase
class Interes : public Inversion {
    protected:
    double tasaInteres;
    double ahorro;

    public:
    Interes(double _capital, int _tiempo, double _ahorro, double _tasaInteres) : Inversion(_capital, _tiempo), tasaInteres(_tasaInteres), ahorro(_ahorro) {
    }
    ~Interes() override {
    }

    //metodo polimorfico para calcular el interes compuesto
    double rendimiento() const override {
        //r es la tasa de interés anual en decimales
        double r = (tasaInteres/100.0);
        // p es el capital inicial (principal)
        double p = capital;
        //vf es el valor futuro de la inversión
        double vf = 0;
        // pmt es el valor de la contribución mensual
        double pmt = ahorro;
        // t es el tiempo en años
        int t = tiempo;
        //formula es VF = P(1 + r/n)^(t) + PMT[((1 + r/n)^(t) - 1) / (r/n)]
        vf=(p*pow((1+(r/12.0)),12*t))+(pmt*(((pow((1+r/12.0),12*t))-1)/(r/12.0)));
        return vf;
    }

    //metodo polimorfico para mostrar el rendimiento con interés compiuesto
    void mostrar() const override {
        cout << "\nInteres Compuesto" << endl;
        Inversion::mostrar();
        cout << "Aporte mensual: $" << ahorro << endl;
        cout << "Rendimiento: $" << rendimiento() << endl;
    }
};

//tercera sublclase
class CuentaAhorros : public Inversion {
    protected:
    double ahorroMensual;

    public:
    CuentaAhorros(double _capital, int _tiempo, double _ahorroMensual) : Inversion(_capital, _tiempo), ahorroMensual(_ahorroMensual) {
    }
    ~CuentaAhorros() override {
    }

    //el metodo polimorfico calcula un balance de ahorros simple (sin tasa de rendimiento)
    double rendimiento() const override {
        //solo suma el capital por el ahorro mensual multiplicado por los años
        return capital + ahorroMensual * 12 * tiempo;
    }

    //este metodo muestra el balance total sumando todos los rendimientos que el usuario agregue
    void mostrar() const override {
        Inversion::mostrar();
        cout << "Total en la cuenta de ahorros: $" << rendimiento() << "\n" << endl;
    }

    //sirve para guardar rendimientos de CETES y de interés compuesto, solo recibe un numero y lo suma al capital inicial
    void depositarGanancias(double ganancia) {
        capital += ganancia;
    }
};

//la clase más importante que guarda toda la información
class Portafolio {
    protected:
    //una cuenta de tipo CuentaAhorros que tiene el capital, el tiempo, la ganancia y el ahorro mensual
    CuentaAhorros cuenta;
    //un vector en el que se suman todas las ganancias y qque usa referencias para no sobrecargar la memoria
    vector<Inversion*> agregadas;
    string nombre;

    public:
    //constructor
    Portafolio(const string &_nombre, const CuentaAhorros &_cuenta) : nombre(_nombre), cuenta(_cuenta) {
    }
    //destructor
    ~Portafolio() {
    }

    //este metodo usa push_back (append en python) para agregar dinero al portafolio
    void agregarInversion(Inversion &_inversion) {
        //ganancia guarda el capital inicial que usa el metodo de la superclase y luego guarda los rendimientos de las subclases
        double ganancia=_inversion.rendimiento();
        //se usa el metodo de la subclase CuentaAhorros para guardar la variable ganancia y que se imprima todo junto
        cuenta.depositarGanancias(ganancia);
        //se agrega el capital inicial o los rendimientos al vector agregadas
        agregadas.push_back(&_inversion);
    }

    //este metodo imprime todo
    void mostrarTodo() {
        cout << nombre << endl;
        cuenta.mostrar();
        //por cada objeto en el vector agregadas, se muestra el capital y lo demás
        for (const Inversion*i: agregadas) {
            i->mostrar();
        }
    }
};

//la clase usuario se usa para la interfaz, para que cada usuario cree y guarde su portafolio, cuenta, etc
class Usuario {
protected:
    string nombre;
    Portafolio portafolio;
public:
    Usuario(const string &_nombre, const Portafolio &_portafolio)
        : nombre(_nombre), portafolio(_portafolio) {
    }
    ~Usuario() {
    }
    Portafolio &getPortafolio() {
        return portafolio;
    }
    void mostrarPortafolio() {
        cout << "\n" << nombre << endl;
        portafolio.mostrarTodo();
    }
};

//main es un menú de tipo switch
int main() {
    double _capital,_ahorro;
    int _tiempo;
    string _nombreusuario;

    //primero se guarda la informacion del usuario
    cout << "Bienvenido al portafolio de inversiones" << endl;
    cout << "Ingresa tu nombre: " << endl;
    cin >> _nombreusuario;
    cout << "Ingresa tu capital inicial: " << endl;
    cin >> _capital;
    cout << "Ingresa tu ahorro mensual: " << endl;
    cin >> _ahorro;
    cout << "Ingresa el periodo de inversion en anos: " << endl;
    cin >> _tiempo;

    //luego se crean los objetos correspondientes de cada clase
    CuentaAhorros cuenta(_capital,_tiempo, _ahorro);
    Portafolio portafolio("Portafolio de inversiones", cuenta);
    Usuario usuario(_nombreusuario, portafolio);

    //menu
    int opcion;
    do {
        cout << "\nIngresa una opcion: " << endl;
        cout << "1. Mostrar portafolio, 2. Agregar CETES, 3. Agregar interes compuesto, 4. Salir" << endl;
        cin >> opcion;
        switch (opcion) {
            case 1:
                usuario.mostrarPortafolio();
                break;
            case 2: {
                double capital, tasa;
                int tiempo;
                cout << "Inversion inicial: " << endl;
                cin >> capital;
                cout << "Ingresa la tasa (7.4 anual para CETES): " << endl;
                cin >> tasa;
                cout << "Ingresa el periodo en anos: " << endl;
                cin >> tiempo;
                Inversion *nueva = new CETES(capital, tiempo, tasa);
                usuario.getPortafolio().agregarInversion(*nueva);
                break;
            }
            case 3: {
                double capital, tasa, ahorro;
                int meses;
                cout << "Inversion inicial: " << endl;
                cin >> capital;
                cout << "Ingresa la tasa de interes: " << endl;
                cin >> tasa;
                cout << "Ingresa el periodo en anos: " << endl;
                cin >> meses;
                cout << "Contribucion mensual: " << endl;
                cin >> ahorro;
                Inversion *nueva = new Interes(capital, meses, ahorro, tasa);
                usuario.getPortafolio().agregarInversion(*nueva);
                break;
            }
        }
    } while (opcion != 4);
    return 0;
}

//
// Created by Luna on 11/29/2025.
//
