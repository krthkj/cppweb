#include "crow_all.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::cursor;

using namespace std;
using namespace crow;
using namespace boost::filesystem;

void sendFile(response &res, string filename, string contentType){
  std::ifstream inIfStream ("/usr/src/cppweb/hello_crow/public/" + filename, std::ifstream::in);
  if(inIfStream){
    ostringstream contents;
    contents << inIfStream.rdbuf();
    inIfStream.close();
    res.set_header("Content-Type", contentType);
    res.write(contents.str());
  } else {
    res.code = 404;
    res.write("Not found");
  }
  res.end();
}

void sendHtml(response &res, string filename){
  sendFile(res, filename + ".html", "text/html");
}

void sendImage(response &res, string filename){
  sendFile(res, "images/" + filename, "image/jpeg");
}

void sendScript(response &res, string filename){
  sendFile(res, "scripts/" + filename, "text/javascript");
}

void sendStyle(response &res, string filename){
  sendFile(res, "styles/" + filename, "text/css");
}

int main(int argc, char* argv[]) {
  crow::SimpleApp app;
  cout << "Current path is " << current_path() << endl;

  mongocxx::instance inst{};
  string mongoConnect = std::string(getenv("MONGODB_URI"));
  mongocxx::client conn{mongocxx::uri{mongoConnect}};
  auto collection = conn["musicDB"]["hashCollection"];

  //CROW_ROUTE(app, "/testdb")
  CROW_ROUTE(app, "/")
    ([&collection](){
      mongocxx::options::find opts;
      opts.limit(25);
      auto docs = collection.find({}, opts);
      std::ostringstream os;
      for(auto &&doc : docs){
        //cout << bsoncxx::to_json(doc) << "\n";
        os << bsoncxx::to_json(doc) << "\n";
      }
      return crow::response(os.str());
    });

  CROW_ROUTE(app, "/test")
    ([](const request &req, response &res){
      sendHtml(res, "index");
    });

  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
