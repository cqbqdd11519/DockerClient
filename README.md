C++ Docker Client
===========================

## Dependency
 - [libcurl](https://curl.haxx.se/libcurl/)
 - [JSON++](https://github.com/hjiang/jsonxx)

## Basic Usage
```C++
#include "docker.h"
int main(){
    Docker client = Docker();
    std::cout << client.list_containers(true).json() << std::endl;
    client.terminate();
    return 0;
}
```

## Return of Each Methods
Every return type is **jsonxx::Object** (refer to [JSON++](https://github.com/hjiang/jsonxx) for its usage)
```
Object o;
 - success        [bool]                  : if succeeded to request
 - data           [Object/Array/string]   : actual data by server
 - code(optional) [int]                   : http status code if 'success' is false
```
Data type of 'data' depends on API, but it would be Object if 'success' is false.
#### e.g.
```JSON
{
  "success": false,
  "code": 404,
  "data": {
      "message": "No such container: 5d271b3a52263330348b71948bd25cda455a49f7e7d69cfc73e6b2f3b5b41a4c" 
  }
} 
```
```JSON
{
  "success": true ,
  "data": {
      "Architecture": "x86_64",
      "SystemTime": "2018-05-23T19:26:54.357768797+09:00" 
 }
}
```

## Accessing Remote Docker Server
For remote access, you sould first bind Docker Server to a port.
You can bind by adding **-H tcp://0.0.0.0:\<port\>** in service daemon.
```bash
# cat /lib/systemd/system/docker.service
....
[Service]
Type=notify
ExecStart=/usr/bin/docker daemon -H fd:// -H tcp://0.0.0.0:4000
....
```

```C++
#include "docker.h"
int main(){
    Docker client = Docker("http://<ip>:<port>");
    std::cout << client.list_containers(true).json() << std::endl;
    client.terminate();
    return 0;
}
```

## API List
#### System
 - system_info
 - docker_version
#### Image
 - list_images
#### Containers
 - list_containers
 - inspect_container
 - top_container
 - logs_container
 - create_container
 - start_container
 - get_container_changes
 - stop_container
 - kill_container
 - pause_container
 - wait_container
 - delete_container
 - unpause_container
 - restart_container
 - attach_to_container
