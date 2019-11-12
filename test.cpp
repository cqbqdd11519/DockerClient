#include "docker.h"

int main(){
    std::string test_id("35d4e9a5886c");

    JSON_DOCUMENT doc;
    Docker client = Docker();
    /*
    *  START testing individual APIs
    */
    //doc = client.system_info();
    //doc = client.docker_version();

    //doc = client.list_images();

    //doc = client.list_containers(true);
    //doc = client.start_container(test_id);
    //doc = client.inspect_container(test_id);
    //doc = client.top_container(test_id);
    //doc = client.logs_container(test_id);

    //rapidjson::Document param(rapidjson::kObjectType);
    //rapidjson::Value commands(rapidjson::kArrayType);
    //commands.PushBack("/bin/bash", param.GetAllocator());
    //param.AddMember("Cmd", commands, param.GetAllocator());
    //param.AddMember("Image", "ubuntu:16.04", param.GetAllocator());
    //doc = client.create_container(param);

    //doc = client.start_container(test_id);
    //doc = client.get_container_changes(test_id);
    //doc = client.stop_container(test_id);
    //doc = client.kill_container(test_id);
    //doc = client.pause_container(test_id);
    //doc = client.wait_container(test_id);
    //doc = client.delete_container(test_id);
    //doc = client.unpause_container(test_id);
    //doc = client.restart_container(test_id);
    //doc = client.attach_to_container(test_id);

    std::cout << jsonToString(doc) << std::endl;
    /*
    *  END testing individual APIs
    */
    return 0;
}

