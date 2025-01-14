# http_server

# about
Basic http server for serving html webpages. User adds html files to *www* folder and run the server. Pages can be accessed by wisiting `http://<serverip>:<port>/html_file_name` (*WITHOUT .html extension in url*).

## tested with [wrk](https://github.com/wg/wrk)
- server(main.cpp) -> listenThreadsNum set to 2 and clientHandleThreadsNum set to 4.
- wrk running on other pc:

command: `./wrk -t6 -c10000 -d30 http://192.168.1.14:9999/` 

output:
```shell
Running 30s test @ http://192.168.1.14:9999/
  6 threads and 10000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    92.38ms   29.33ms 310.91ms   81.65%
    Req/Sec     4.56      3.94    10.00     50.19%
  267 requests in 30.05s, 9.13KB read
  Socket errors: connect 8981, read 71307, write 293, timeout 0
Requests/sec:      8.89
Transfer/sec:     311.00B
```

# setup project
## clone repository
```
git clone https://github.com/IamTV50/http_server
cd http_server
```

## setup
- change `host` to ipv4 of computer server will be running on (in main.cpp)
- (not necessary) change port if you wish (in main.cpp)
- (not necessary) change number of listener and client handeling threads (in HttpServer.h)

rename `index_template.html` to `index.html` and `404_template.html` to `404.html` (both are located in **www** folder).

# note
When pushing changes to git, remember that files inside **www** folder will be **IGNORED** with exception of *index_template.html* and *404_template.html*.
