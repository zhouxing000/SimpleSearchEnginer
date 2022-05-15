#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO();

    int readn(char *buf, int size);
    int readline(char *buf, int size);
    int writen(const char *buf, int size);

private:
    int _fd;
};

#endif