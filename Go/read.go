package main

import (
	"log"
	"net"
	"time"
	"net/http"
)

func serial_read(IP string) {
	dialer := &net.Dialer{
		Timeout:   5 * time.Second,
		KeepAlive: 10 * time.Second,
		DualStack: true,
	}
	log.Println(IP)

	Conn, err := dialer.Dial("tcp", IP)
	if err != nil {
		log.Println(err)
	}

	resp, err := http.Get(IP)
	log.Println(resp.Header, err)
}