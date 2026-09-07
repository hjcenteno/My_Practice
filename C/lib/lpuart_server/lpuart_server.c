/*
    Author: Henry Centeno
    Description:
        Server implementation of the uart_driver. Refer to Embedded_Projects/lib/uart_driver/readme.md for details
        Open and read ttyACM0

    sources:
        *https://en.wikibooks.org/wiki/Serial_Programming/termios
        -Used to how to write the program
        *https://man7.org/linux/man-pages/man0/termios.h.0p.html
        -more details for termios.h
*/

#include "lpuart_server.h"

int init_lpuart_server(void){
    //set up to read the ttyACM0 as this is where the mcu will write to
    //the computer will continously read the ttyACM0, but it will know to read the start of client transmission from the start byte (0xAA)
    //first open /dev/ttyACM0
    int port = open("/dev/ttyACM0", (O_RDWR | O_NOCTTY)); //configure for read/write and that the ttyACM0 does not become the controlling terminal
    if(port < 0){ //check if it open
        perror("unable to open '/dev/ttyACM0'");
        return -1;
    }

    //I plan to use non-canonical mode to read from ttyACM0 since the mcu is sending 1 byte at a time through the lpuart.
    struct termios tty;
    if(tcgetattr(port, &tty) != 0){
        perror("tcgetattr");
        close(port);
        return -1;
    }

    //set up the tty to read from the port
    //set up the baud rate to 9600 to match the baud of the mcu
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    //set up the flags of the tty
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB);
    tty.c_cflag &= ~(CSTOPB);
    tty.c_cflag &= ~(CSIZE);
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~(CRTSCTS);

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(
        IXON | IXOFF | IXANY | IGNBRK |
        BRKINT | PARMRK | ISTRIP | INLCR | 
        IGNCR | ICRNL
    );
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1; //block until at least 1 byte is available
    tty.c_cc[VTIME] = 0;

    if(tcsetattr(port, TCSANOW, &tty) != 0){
        perror("tcsetattr");
        close(port);
        return -1;
    }

    return port;
}

void close_lpuart_server(int fd){
    //closes the server
    if(fd >= 0){
        close(fd);
    }
}

static int read_byte(int fd, uint8_t *byte){ //static to not expose this function publically
    while(1){
        ssize_t n = read(fd, byte, 1); //read the byte
        if(n == 1){ //had read the byte
            return 0;
        }

        if(n == 0){ //did not read the byte
            return -1;
        }

        if(errno == EINTR){
            continue;
        }

        return -1;
    }
}

int run_lpuart_server(int fd, lpuart_data_handler handler){
    //implementation of the state machine, refer to embedded_project/lib/uart_driver for detailes
    if(handler == NULL){ //only run if there is a handler
        return -1;
    }

    uint8_t byte, length, expected_checksum; //states

    while(1){ //continuously read the ttyAMC0
        //reset the actual checksum and rb size between transmissions
        uint8_t actual_checksum = start_byte;
        uint8_t payload[MAX_PAYLOAD_LENGTH];
        
        //read in the first byte
        if(read_byte(fd, &byte) != 0){ //some error happened in reading ttyACMO, return
            return -1;
        }

        //read for the start byte
        if(byte != start_byte){
            continue; //keep reading until the start byte has been read
        }

        //read for the length byte
        if(read_byte(fd, &length) != 0){
            return -1;
        }

        if((length < 1)){
            fprintf(stdout, "length does not fit in the range (0, 255]\n");
            continue; //return to read start byte state
        }

        actual_checksum ^= length; 

        //read the payload
        for(uint8_t i = 0; i < length; i++){ //read N (length) bytes for the data
            if(read_byte(fd, &payload[i]) != 0){
                return -1;
            }

            actual_checksum ^= payload[i];
        }

        //check the checksum
        if(read_byte(fd, &expected_checksum) != 0){
            return -1;
        }

        if(expected_checksum != actual_checksum){
            fprintf(stdout, "[ERROR] expected checksum did not match actual, bad transmission, disregard\n");
            continue;
        }

        //handle the buffer whenever the length is read and passed the checksum
        handler(payload, length);
    }

    return 0;
}