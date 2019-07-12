#include "docker.h"

using namespace std;
    /*
    *  JSON Object / Array Usage
    * e.g.)
    * Object o; / Array o;
    * o << "FIELD_NAME1" << "VALUE1";
    * o << "FIELD_NAME2" << "VALUE2";
    * 
    * if(o.has<String>("FIELD_NAME1"))
    *   string str = o.get<String>("FIELD_NAME1");
    */

string test_id = "5d271b3a52263330348b71948bd25cda455a49f7e7d69cfc73e6b2f3b5b41a4c";

int main(){
    Docker client = Docker();
    /*
    *  START testing individual APIs
    */
    //cout << client.system_info().json() << endl;
    //cout << client.docker_version().json() << endl;

    //cout << client.list_images().json() << endl;

    cout << client.list_containers(true).json() << endl;
    //cout << client.start_container(test_id).json() << endl;
    //cout << client.inspect_container(test_id).json() << endl;
    //cout << client.top_container(test_id).json() << endl;
    //cout << client.logs_container(test_id) << endl;
    
    //Array commands;
    //commands << "/bin/bash";
    //Object createObj;
    //createObj << "Cmd" << commands;
    //createObj << "Image" << "ubuntu:16.04";
    //cout << client.create_container(createObj).json() << endl;

    //cout << client.start_container(test_id).json() << endl;
    //cout << client.get_container_changes(test_id).json() << endl;
    //cout << client.stop_container(test_id).json() << endl;
    //cout << client.kill_container(test_id).json() << endl;
    //cout << client.pause_container(test_id).json() << endl;
    //cout << client.wait_container(test_id).json() << endl;
    //cout << client.delete_container(test_id).json() << endl;
    //cout << client.unpause_container(test_id).json() << endl;
    //cout << client.restart_container(test_id).json() << endl;
    //cout << client.attach_to_container(test_id).json() << endl;

    /*
    *  END testing individual APIs
    */
    client.terminate();
    return 0;
}

