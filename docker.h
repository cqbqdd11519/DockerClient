#include <iostream>
#include <cstdlib>
#include <string.h>
#include <curl/curl.h>
#include "jsonxx/jsonxx.h"

#define JSON_OBJECT jsonxx::Object
#define JSON_ARRAY jsonxx::Array

typedef enum{
    GET,
    POST,
    DELETE,
    PUT
} Method;

std::string param( std::string param_name, std::string param_value);
std::string param( std::string param_name, const char* param_value);
std::string param( std::string param_name, bool param_value);
std::string param( std::string param_name, int param_value);
std::string param( std::string param_name, JSON_OBJECT param_value);


class Docker{
    public :
        Docker();
        Docker(std::string host);

        /*
        * System
        */
        JSON_OBJECT system_info();
        JSON_OBJECT docker_version();

        /*
        * Images
        */
        JSON_OBJECT list_images();

        /*
        * Containers
        */
        JSON_OBJECT list_containers(bool all=false, int limit=-1, std::string since="", std::string before="", int size=-1, JSON_OBJECT filters=JSON_OBJECT());
        JSON_OBJECT inspect_container(std::string container_id);
        JSON_OBJECT top_container(std::string container_id);
        JSON_OBJECT logs_container(std::string container_id, bool follow=false, bool o_stdout=true, bool o_stderr=false, bool timestamps=false, std::string tail="all");
        JSON_OBJECT create_container(JSON_OBJECT parameters);
        JSON_OBJECT start_container(std::string container_id);
        JSON_OBJECT get_container_changes(std::string container_id);
        JSON_OBJECT stop_container(std::string container_id, int delay=-1);
        JSON_OBJECT kill_container(std::string container_id, int signal=-1);
        JSON_OBJECT pause_container(std::string container_id);
        JSON_OBJECT wait_container(std::string container_id);
        JSON_OBJECT delete_container(std::string container_id, bool v=false, bool force=false);
        JSON_OBJECT unpause_container(std::string container_id);
        JSON_OBJECT restart_container(std::string container_id, int delay=-1);
        JSON_OBJECT attach_to_container(std::string container_id, bool logs=false, bool stream=false, bool o_stdin=false, bool o_stdout=false, bool o_stderr=false);
        //void copy_from_container(std::string container_id, std::string file_path, std::string dest_tar_file);

        void terminate();

    private:
        std::string host_uri;
        bool is_remote;
        CURL *curl;
        CURLcode res;

        template <typename paramT> JSON_OBJECT requestAndParse(Method method, std::string path, long success_code = 200, paramT param=paramT(), bool isReturnJson=false);
        template <typename paramT, typename returnT> JSON_OBJECT requestAndParseJson(Method method, std::string path, long success_code = 200, paramT param=paramT());

        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
};