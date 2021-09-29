package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"github.com/gorilla/mux"
	"io/ioutil"
	"log"
	"net/http"
	"strconv"
	"time"
)

type ClimateData struct {
	T float64 `json:"t"`
	H float64 `json:"h"`
}

type ClimateConfig struct {
	DayMaxTemp             float64 `json:"day_max_temp"`
	NightMaxTemp           float64 `json:"night_max_temp"`
	DayTempToleranceWarm   float64 `json:"day_temp_tolerance_warm"`
	NightTempToleranceWarm float64 `json:"night_temp_tolerance_warm"`
	DayTempToleranceCold   float64 `json:"day_temp_tolerance_cold"`
	NightTempToleranceCold float64 `json:"night_temp_tolerance_cold"`
}

type System struct {
	Version float64 `json:"version"`
	Uptime  float64 `json:"uptime"`
	Hour    float64 `json:"hour"`
	Minute  float64 `json:"minute"`
	Second  float64 `json:"second"`
}

type Data struct {
	HotSide     ClimateData   `json:"hot_side"`
	HotCenter   ClimateData   `json:"hot_center"`
	ColdCenter  ClimateData   `json:"cold_center"`
	ColdSide    ClimateData   `json:"cold_side"`
	Heater      bool          `json:"heater"`
	Config      ClimateConfig `json:"climate_config"`
	HeaterPhase string        `json:"heater_phase"`
}

var errResponse struct {
	Message string `json:"error"`
}

type Storage struct {
	Data      Data   `json:"data"`
	Timestamp int64  `json:"timestamp"`
	System    System `json:"system"`
}

var storage map[int]Storage

var debug = true

func recordTelemetry(w http.ResponseWriter, req *http.Request) {

	vars := mux.Vars(req)
	listIdString := vars["id"]

	if listIdString == "" {
		listIdString = "0"
	}

	id, err := strconv.Atoi(listIdString)

	if err != nil {
		fmt.Println("error", err)
		return
	}

	if debug {

		buf, bodyErr := ioutil.ReadAll(req.Body)
		if bodyErr != nil {
			log.Print("bodyErr ", bodyErr.Error())
			http.Error(w, bodyErr.Error(), http.StatusInternalServerError)
			return
		}

		rdr1 := ioutil.NopCloser(bytes.NewBuffer(buf))
		rdr2 := ioutil.NopCloser(bytes.NewBuffer(buf))
		log.Printf("BODY: %q", rdr1)
		req.Body = rdr2
	}

	var telemetry Data
	err = json.NewDecoder(req.Body).Decode(&telemetry)

	if err != nil {
		errResponse.Message = fmt.Sprintf("error on parsing payload, %v", err)
		json.NewEncoder(w).Encode(errResponse)
		return
	}

	storage[id] = Storage{
		Data:      telemetry,
		Timestamp: time.Now().Unix(),
	}

	fmt.Fprintf(w, "ok\n")
}

func printTelemetry(w http.ResponseWriter, req *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(storage)
}

func main() {

	storage = make(map[int]Storage)

	muxRouter := mux.NewRouter()

	muxRouter.HandleFunc("/api/v1/telemetry", recordTelemetry).Methods("POST")
	muxRouter.HandleFunc("/api/v1/telemetry", printTelemetry).Methods("GET")

	muxRouter.HandleFunc("/api/v2/telemetry/{id}", recordTelemetry).Methods("POST")
	muxRouter.HandleFunc("/api/v2/telemetry", printTelemetry).Methods("GET")

	err := http.ListenAndServe(":80", muxRouter)

	if err != nil {
		panic(err)
	}
}
