#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <stdexcept> 

std::mutex mtx;  
std::vector<std::string> resultados; 

// n aleatorio
int generarNumeroAleatorio(int min, int max) {
static thread_local std::mt19937 generador(std::random_device{}());
std::uniform_int_distribution<int> distribucion(min, max);
return distribucion(generador);
}

// auto
struct Auto {
std::string nombre;
int distancia_total;
int distancia_avanzada = 0;

    Auto(std::string nombre, int distancia_total) : nombre(nombre), distancia_total(distancia_total) {}
    
    // avanzar
    void correr() {
    while (distancia_avanzada < distancia_total) {
        int avance = generarNumeroAleatorio(1, 10);
        distancia_avanzada += avance;
        if (distancia_avanzada > distancia_total) {
            distancia_avanzada = distancia_total;
        }

            // tiempo de espera
        int tiempo_espera = generarNumeroAleatorio(100, 500);
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo_espera));

            // avance
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << nombre << " ha avanzado " << avance << " metros, total: " << distancia_avanzada << "/" << distancia_total << " metros.\n";
        }

        // llegada del auto
        {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << nombre << " ha terminado la carrera.\n";
        resultados.push_back(nombre); 
        }
    }
};

// validar n entero
bool esEnteroValido(const std::string& str) {
    
if (str.empty()) return false;

    size_t inicio = 0;
if (str[0] == '-') {
     if (str.size() == 1) return false;  
        inicio = 1; 
    }

for (size_t i = inicio; i < str.size(); ++i) {
    if (!std::isdigit(str[i])) {
        return false; 
    }
}

return true;
}

// start
void iniciarCarrera(int num_autos, int distancia_total) {
std::vector<std::thread> hilos;
std::vector<Auto> autos;

for (int i = 1; i <= num_autos; ++i) {
    autos.emplace_back("Auto" + std::to_string(i), distancia_total);
}

for (int i = 0; i < num_autos; ++i) {
    hilos.emplace_back(&Auto::correr, &autos[i]);
}

// esperar
for (auto &hilo : hilos) {
     hilo.join();
}

 // resultados finales
std::lock_guard<std::mutex> lock(mtx);
std::cout << "\n--- Resultados Finales ---\n";
for (size_t i = 0; i < resultados.size(); ++i) {
    std::cout << i + 1 << ". " << resultados[i] << " ha llegado a la meta.\n";
    }
}

int main() {
    int num_autos = 0;
    int distancia_total = 0;

    // loop validacion n autos
    while (true) {
        std::string input_num_autos;
        std::cout << "Ingrese el número de autos : ";
        std::getline(std::cin, input_num_autos);

        if (esEnteroValido(input_num_autos)) {
            num_autos = std::stoi(input_num_autos);
            if (num_autos > 1) break; 
        }
        std::cerr << "Error: El número de autos debe ser un entero mayor que 1.\n";
    }
// loop validacion distancia
    while (true) {
        std::string input_distancia;
        std::cout << "Ingrese la distancia total en metros : ";
        std::getline(std::cin, input_distancia);

        if (esEnteroValido(input_distancia)) {
            distancia_total = std::stoi(input_distancia);
            if (distancia_total > 0) break; 
        }
        std::cerr << "Error: La distancia debe ser un entero mayor que 0.\n";
    }

    // iniciar 
    iniciarCarrera(num_autos, distancia_total);

    return 0;
}
