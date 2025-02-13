package main

import (
	"fmt"
	"log"
	"go.bug.st/serial"
)

func main() {
	//---------------------------------------------------------------------
	
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}
	for i, port := range ports {
		go serial_read(port, i)
	}

	//---------------------------------------------------------------------
	
	mode := &serial.Mode{
		BaudRate: 9600,
		Parity: serial.EvenParity,
		DataBits: 7,
		StopBits: serial.OneStopBit,
	}
	port, err := serial.Open("?-------?", mode)
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

	//---------------------------------------------------------------------
}