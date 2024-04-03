#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;

int main() {
    // Crear un listener HTTP en el puerto 8080
    http_listener listener(U("http://0.0.0.0:8080/test"));

    // Manejar las solicitudes GET en el endpoint /test
    listener.support(methods::GET, [](http_request request) {
        // Crear una respuesta con el código de estado OK (200) y un cuerpo en formato JSON
        web::json::value response_body;

        std::map<std::string, std::string> my_map = {{"Nombre", "Gustavo"}, {"Apellido", "Fresno"}, {"Edad", "40"}};
        for (const auto& pair : my_map) {
            response_body[pair.first] = web::json::value::string(pair.second);
        }
        
        http_response response(status_codes::OK);
        response.set_body(response_body);

        // Enviar la respuesta al cliente
        request.reply(response);
    });

    // Iniciar el listener
    try {
        listener.open().wait();
        std::cout << "Listening for requests at: " << listener.uri().to_string() << std::endl;
        std::cin.get();
        listener.close().wait();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}