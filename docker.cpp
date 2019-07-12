/****
 Dependency
 - libcurl
 - [JSON++](https://github.com/hjiang/jsonxx)



 Basic Usage

#include "docker.h"
int main(){
    Docker client = Docker();
    std::cout << client.list_containers(true).json() << std::endl;
    client.terminate();
    return 0;
} 



 Return Type for Each Methods

Object o; (jsonxx::Object)
 - success        [bool]                  : if succeeded to request
 - data           [Object/Array/string]   : actual data by server (data type depends on API, but it would be Object if 'success' is false)
 - code(optional) [int]                   : http status code if 'success' is false

e.g.
{
  "success": false,
  "code": 404,
  "data": {
      "message": "No such container: 5d271b3a52263330348b71948bd25cda455a49f7e7d69cfc73e6b2f3b5b41a4c" 
  }
} 


{
  "success": true ,
  "data": {
      "Architecture": "x86_64",
          ...
      "SystemTime": "2018-05-23T19:26:54.357768797+09:00" 
  }
}



 JSON Object / Array Usage

Object o; / Array o;
o << "FIELD_NAME1" << "VALUE1";
o << "FIELD_NAME2" << "VALUE2";

if(o.has<String>("FIELD_NAME1"))
  string str = o.get<String>("FIELD_NAME1");

****/

#include "docker.h"

/*
*  
* START Docker Implementation
* 
*/
Docker::Docker() : host_uri("http:/v1.24"){
    curl_global_init(CURL_GLOBAL_ALL);
    is_remote = false;
}
Docker::Docker(std::string host) : host_uri(host){
    curl_global_init(CURL_GLOBAL_ALL);
    is_remote = true;
}

/*
*  
* Public Methods
* 
*/

/*
* System
*/
JSON_OBJECT Docker::system_info(){
    std::string path = "/info";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(GET,path);
}
JSON_OBJECT Docker::docker_version(){
    std::string path = "/version";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(GET,path);
}

/*
* Images
*/
JSON_OBJECT Docker::list_images(){
    std::string path = "/images/json";
    return requestAndParseJson<JSON_OBJECT,JSON_ARRAY>(GET,path);
}

/*
* Containers
*/
JSON_OBJECT Docker::list_containers(bool all, int limit, std::string since, std::string before, int size, JSON_OBJECT filters){
    std::string path = "/containers/json?";
    path += param("all", all);
    path += param("limit", limit);
    path += param("since", since);
    path += param("before", before);
    path += param("size", size);
    path += param("filters", filters);
    return requestAndParseJson<JSON_OBJECT,JSON_ARRAY>(GET,path);
}
JSON_OBJECT Docker::inspect_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/json";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(GET,path);
}
JSON_OBJECT Docker::top_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/top";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(GET,path);
}
JSON_OBJECT Docker::logs_container(std::string container_id, bool follow, bool o_stdout, bool o_stderr, bool timestamps, std::string tail){
    std::string path = "/containers/" + container_id + "/logs?";
    path += param("follow", follow);
    path += param("stdout", o_stdout);
    path += param("stderr", o_stderr);
    path += param("timestamps", timestamps);
    path += param("tail", tail);
    return requestAndParse<JSON_OBJECT>(GET,path,101);
}
JSON_OBJECT Docker::create_container(JSON_OBJECT parameters){
    std::string path = "/containers/create";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(POST,path,201,parameters);
}
JSON_OBJECT Docker::start_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/start";
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::get_container_changes(std::string container_id){
    std::string path = "/containers/" + container_id + "/changes";
    return requestAndParseJson<JSON_OBJECT,JSON_ARRAY>(GET,path);
}
JSON_OBJECT Docker::stop_container(std::string container_id, int delay){
    std::string path = "/containers/" + container_id + "/stop?";
    path += param("t", delay);
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::kill_container(std::string container_id, int signal){
    std::string path = "/containers/" + container_id + "/kill?";
    path += param("signal", signal);
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::pause_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/pause";
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::wait_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/wait";
    return requestAndParseJson<JSON_OBJECT,JSON_OBJECT>(POST,path);
}
JSON_OBJECT Docker::delete_container(std::string container_id, bool v, bool force){
    std::string path = "/containers/" + container_id + "?";
    path += param("v", v);
    path += param("force", force);
    return requestAndParse<JSON_OBJECT>(DELETE,path,204);
}
JSON_OBJECT Docker::unpause_container(std::string container_id){
    std::string path = "/containers/" + container_id + "/unpause?";
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::restart_container(std::string container_id, int delay){
    std::string path = "/containers/" + container_id + "/restart?";
    path += param("t", delay);
    return requestAndParse<JSON_OBJECT>(POST,path,204);
}
JSON_OBJECT Docker::attach_to_container(std::string container_id, bool logs, bool stream, bool o_stdin, bool o_stdout, bool o_stderr){
    std::string path = "/containers/" + container_id + "/attach?";
    path += param("logs", logs);
    path += param("stream", stream);
    path += param("stdin", o_stdin);
    path += param("stdout", o_stdout);
    path += param("stderr", o_stderr);

    return requestAndParse<JSON_OBJECT>(POST,path,101);
}
//void Docker::copy_from_container(std::string container_id, std::string file_path, std::string dest_tar_file){}


void Docker::terminate(){
    curl_global_cleanup();
}

/*
*  
* Private Methods
* 
*/

template <typename paramT>
JSON_OBJECT Docker::requestAndParse(Method method, std::string path, long success_code, paramT param, bool isReturnJson){
    std::string readBuffer;
    std::string paramString;
    std::string method_str;
    struct curl_slist *headers = NULL;
    const char *paramChar;
    switch(method){
        case GET:
            method_str = "GET";
            break;
        case POST:
            method_str = "POST";
            break;
        case DELETE:
            method_str = "DELETE";
            break;
        case PUT:
            method_str = "PUT";
            break;
        default:
            method_str = "GET";
    }

    curl = curl_easy_init();
    if(!curl){
        std::cout << "error while initiating curl" << std::endl;
        curl_global_cleanup();
        exit(1);
    }
    paramString = param.json();
    paramChar = paramString.c_str();
    if(isReturnJson)
        headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    //std::cout << "HOST_PATH : " << (host_uri + path) << std::endl;

    if(!is_remote)
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock");
    curl_easy_setopt(curl, CURLOPT_URL, (host_uri + path).c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method_str.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    if(method == POST){
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paramChar);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(paramChar));
    }

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    long status = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &status);
    curl_easy_cleanup(curl);
    //std::cout << readBuffer << std::endl;
    JSON_OBJECT o;
    if(status == success_code || status == 200){
        o << "success" << true;
        o << "data" << readBuffer;
    }else{
        JSON_OBJECT err;
        err.parse(readBuffer);
        o << "success" << false;
        o << "code" << status;
        o << "data" << err;
    }
    return o;
}

template <typename paramT, typename returnT>
JSON_OBJECT Docker::requestAndParseJson(Method method, std::string path, long success_code, paramT param){
    JSON_OBJECT result_obj = requestAndParse<paramT>(method,path,success_code,param,true);
    bool result = result_obj.get<bool>("success");
    if(result){
        returnT o;
        o.parse(result_obj.get<std::string>("data"));

        JSON_OBJECT out;
        out << "success" << true;
        out << "data" << o;
        //cout << o.json() << endl;
        return out;
    }else{
        return result_obj;
    }
}

/*
*  
* END Docker Implementation
* 
*/

std::string param( std::string param_name, std::string param_value){
    if(!param_value.empty()){
        return "&" + param_name + "=" + param_value;
    }
    else{
        return "";
    }
}

std::string param( std::string param_name, const char* param_value){
    if(param_value != NULL){
        return "&" + param_name + "=" + param_value;
    }
    else{
        return "";
    }
}

std::string param( std::string param_name, bool param_value){
    std::string ret;
    ret = "&" + param_name + "=";
    if(param_value){
        return ret + "true";
    }
    else{
        return ret + "false";
    }
}

std::string param( std::string param_name, int param_value){
    if(param_value != -1){
        std::ostringstream convert;
        convert << param_value;
        return "&" + param_name + "=" + convert.str();
    }
    else{
        return "";
    }
}

std::string param( std::string param_name, JSON_OBJECT param_value){
    if(!param_value.empty()){
        return "&" + param_name + "=" + param_value.json();
    }
    else{
        return "";
    }
}

