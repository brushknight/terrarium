package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"github.com/gorilla/mux"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

type ClimateData struct {
	T float64 `json:"t"`
	H float64 `json:"h"`
}

type ClimateConfig struct {
	DayMaxTemp         float64 `json:"day_max_temp"`
	NightMaxTemp       float64 `json:"night_max_temp"`
	DayTempTolerance   float64 `json:"day_temp_tolerance"`
	NightTempTolerance float64 `json:"night_temp_tolerance"`
}

type Data struct {
	HotSide    ClimateData   `json:"hot_side"`
	HotCenter  ClimateData   `json:"hot_center"`
	ColdCenter ClimateData   `json:"cold_center"`
	ColdSide   ClimateData   `json:"cold_side"`
	Heater     bool          `json:"heater"`
	Config     ClimateConfig `json:"climate_config"`
}

var errResponse struct {
	Message string `json:"error"`
}

type Storage struct {
	Data      Data  `json:"data"`
	Timestamp int64 `json:"timestamp"`
}

var storage Storage

var debug = true

func recordTelemetry(w http.ResponseWriter, req *http.Request) {

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
	err := json.NewDecoder(req.Body).Decode(&telemetry)

	if err != nil {
		errResponse.Message = fmt.Sprintf("error on parsing payload, %v", err)
		json.NewEncoder(w).Encode(errResponse)
		return
	}

	storage.Data = telemetry
	storage.Timestamp = time.Now().Unix()

	fmt.Fprintf(w, "ok\n")
}

func printTelemetry(w http.ResponseWriter, req *http.Request) {
	json.NewEncoder(w).Encode(storage)
}

func main() {

	muxRouter := mux.NewRouter()

	muxRouter.HandleFunc("/api/v1/telemetry", recordTelemetry).Methods("POST")
	muxRouter.HandleFunc("/api/v1/telemetry", printTelemetry).Methods("GET")

	http.ListenAndServe(":80", muxRouter)
}
