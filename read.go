package main

import (
	"fmt",
	"log"
)

func serial_read(port string, i int) {
	mode := &serial.Mode{
		BaudRate: 9600,
		Parity: serial.EvenParity,
		DataBits: 7,
		StopBits: serial.OneStopBit,
	}
	open_port, err := serial.Open(port, mode)
	if err != nil {
		log.Fatal(err)
	}

	//---------------------------------------------------------------------
	
	buff := make([]byte, 100)
	for {
		n, err := port.Read(buff)
		if err != nil {
			log.Fatal(err)
			break
		}
		if n == 0 {
			fmt.Println("\nEOF")
			break
		}
		fmt.Printf("%v", string(buff[:n]))
	}
}