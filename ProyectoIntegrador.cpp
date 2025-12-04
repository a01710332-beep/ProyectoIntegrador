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
    Inversion(double _capital, int _tiempo): capital(_capital), tiempo(_tiempo) {
    }
    virtual ~Inversion() {
    }
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
    virtual double rendimiento() const {
        return capital;
    }
    virtual void mostrar() const {
        cout << "Capital invertido: $" << capital << " Tiempo: " << tiempo << " anos\n" << endl;
    }
};

class CETES : public Inversion {
protected:
    double tasaCETES;

public:
    CETES(double _capital, int _tiempo, double _tasaCETES) : Inversion(_capital, _tiempo), tasaCETES(_tasaCETES){
    }
    ~CETES() override {
    }
    double rendimiento() const override {
        double tasa = (tasaCETES / 100.0);
        double rend = capital *(1+tasa)*tiempo;
        return rend;
    }
    void mostrar() const override {
        cout << "CETES" << endl;
        Inversion::mostrar();
        cout << "Rendimiento: $" << rendimiento() << "\n" << endl;
    }
};


class Interes : public Inversion {
protected:
    double tasaInteres;
    double ahorro;

public:
    Interes(double _capital, int _tiempo, double _ahorro, double _tasaInteres) : Inversion(_capital, _tiempo), tasaInteres(_tasaInteres), ahorro(_ahorro) {
    }
    ~Interes() override {
    }
    double rendimiento() const override {
        double r = (tasaInteres/100.0);
        double p = capital;
        double vf = 0;
        double pmt = ahorro;
        int t = tiempo;
        vf=(p*pow((1+(r/12.0)),12*t))+(pmt*(((pow((1+r/12.0),12*t))-1)/(r/12.0)));
        return vf;
    }
    void mostrar() const override {
        cout << "\nInteres Compuesto" << endl;
        Inversion::mostrar();
        cout << "Aporte mensual: $" << ahorro << endl;
        cout << "Rendimiento: $" << rendimiento() << endl;
    }
};


class CuentaAhorros : public Inversion {
protected:
    double ahorroMensual;
public:
    CuentaAhorros(double _capital, int _tiempo, double _ahorroMensual) : Inversion(_capital, _tiempo), ahorroMensual(_ahorroMensual) {
    }
    ~CuentaAhorros() override {
    }
    double rendimiento() const override {
        return capital + ahorroMensual * 12 * tiempo;
    }
    void mostrar() const override {
        Inversion::mostrar();
        cout << "Total en la cuenta de ahorros: $" << rendimiento() << "\n" << endl;
    }
    void depositarGanancias(double ganancia) {
        capital += ganancia;
    }
};

class Portafolio {
protected:
    CuentaAhorros cuenta;
    vector<Inversion*> agregadas;
    string nombre;
public:
    Portafolio(const string &_nombre, const CuentaAhorros &_cuenta) : nombre(_nombre), cuenta(_cuenta) {
    }
    ~Portafolio() {
    }
    void agregarInversion(Inversion &_inversion) {
        double ganancia=_inversion.rendimiento();
        cuenta.depositarGanancias(ganancia);
        agregadas.push_back(&_inversion);
    }
    void mostrarTodo() {
        cout << nombre << endl;
        cuenta.mostrar();
        for (const Inversion*i: agregadas) {
            i->mostrar();
        }
    }
};

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


int main() {
    double c,a;
    int t;
    string n;
    cout << "Bienvenido al portafolio de inversiones" << endl;
    cout << "Ingresa tu nombre: " << endl;
    cin >> n;
    cout << "Ingresa tu capital inicial: " << endl;
    cin >> c;
    cout << "Ingresa tu ahorro mensual: " << endl;
    cin >> a;
    cout << "Ingresa el periodo de inversion en anos: " << endl;
    cin >> t;
    CuentaAhorros cuenta(c,t, a);
    Portafolio portafolio("Portafolio de inversiones", cuenta);
    Usuario usuario(n, portafolio);
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
