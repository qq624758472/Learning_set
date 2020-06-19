# rm ./client
# rm ./server

# gcc server.c -I/usr/include -L/usr/lib/ -Wl,-rpath /usr/lib -levent_openssl -levent  -o server
# gcc client.c -I/usr/include -L/usr/lib/ -Wl,-rpath /usr/lib -levent_openssl -levent  -o client
gcc server.c -I/usr/include -L/usr/lib/ -Wl,-rpath /usr/lib  -levent -lpthread -lpython2.7 -o server
gcc client.c -I/usr/include -L/usr/lib/ -Wl,-rpath /usr/lib  -levent -lpython2.7 -o client
