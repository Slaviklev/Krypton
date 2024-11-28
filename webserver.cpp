#include <iostream>
#include "include/crow.h"
#include "form.h"

#define ADDRESS "127.0.0.1"
#define PORT     5555

int main(int argc, char* argv[]) {
    crow::SimpleApp app;
	crow::mustache::set_base(".");

	CROW_ROUTE(app, "/")([](){
		//crow::mustache::context ctx;
		std::string path = "web/pages/index.html";
		auto page = crow::mustache::load(path);
        return page.render();
    });

    CROW_ROUTE(app, "/<string>")([](std::string file){
		std::string path = "web/pages/";
		path += file;
		auto page = crow::mustache::load(path);
        return page.render();
    });

	CROW_ROUTE(app, "/styles/<string>")([](std::string file){
		std::string path = "web/styles/";
		path += file;
		auto page = crow::mustache::load_text(path);
		return page;
    });
	
	CROW_ROUTE(app, "/images/<string>")([](std::string file){
		std::string path = "web/images/";
		path += file;
		auto page = crow::mustache::load(path);
		return page.render();
    });

    CROW_ROUTE(app, "/post_sign_up").methods("POST"_method)([](const crow::request& req) {
        auto name = req.body;
		auto form = get_form(name);
		std::string str = form["email"] + " " + form["username"] + " " +  form["password"];
		return str;
    });

	CROW_ROUTE(app, "/post_sign_in").methods("POST"_method)([](const crow::request& req) {
        auto name = req.body;
		auto form = get_form(name);
		std::string str = form["username"] + " " +  form["password"];
		return str;
    });
	
	if (NULL != argv[1]) {
  		app.bindaddr(ADDRESS).port(atoi(argv[1])).multithreaded().run();		
	} else {
		app.bindaddr(ADDRESS).port(PORT).multithreaded().run();
	}
}
