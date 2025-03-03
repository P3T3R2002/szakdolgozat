package main

import (
	"log"
	"fmt"
	"net/http"
	"io/ioutil"
)

var IPAdress = [...]string{"172.28.3.249:8080", "321", "231", "..."}

func main() {
	//---------------------------------------------------------------------

	serveMux := http.NewServeMux()
	serveMux.HandleFunc("/", getData1)
	serveMux.HandleFunc("POST /", getData2)
	serveMux.HandleFunc("GET /", getData3)

	var server = &http.Server{
		Addr: ":8080",
		Handler: serveMux,
	}
	log.Fatal(server.ListenAndServe())

	//---------------------------------------------------------------------
}

func getData1(w http.ResponseWriter, r *http.Request) {
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Body: %v", string(body))
	fmt.Println("Hello, from server!")
}

func getData2(w http.ResponseWriter, r *http.Request) {
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Body: %v", string(body))
	fmt.Println("Hello POST, from server!")
}

func getData3(w http.ResponseWriter, r *http.Request) {
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		log.Fatal(err)
	}
	log.Printf("Body: %v", string(body))
	fmt.Println("Hello, GET from server!")
}
