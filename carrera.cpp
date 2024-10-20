#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <algorithm>

//David Moreno Vicente Bascuñan


std::mutex mtx;  
bool carrera_finalizada = false;  
std::vector<std::string> resultados; 

// numero random
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
    //distancia
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

        // lugar
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << nombre << " ha terminado la carrera.\n";
            resultados.push_back(nombre); 
        }
    }
};

// inicio
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

    // orden de llegada
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\n--- Resultados Finales ---\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        std::cout << i + 1 << ". " << resultados[i] << " ha llegado a la meta.\n";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <numero_de_autos> <distancia_total_en_metros>\n";
        return 1;
    }

    int num_autos = std::stoi(argv[1]);
    int distancia_total = std::stoi(argv[2]);

    iniciarCarrera(num_autos, distancia_total);

    return 0;
}
