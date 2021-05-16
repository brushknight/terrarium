# Dockerfile References: https://docs.docker.com/engine/reference/builder/

### Build binary

# Start from the latest golang base image
FROM golang:latest AS builder

# Add Maintainer Info
LABEL maintainer="Grigorii Merkushev <brushknight@gmail.com>"

# Set the Current Working Directory inside the container
WORKDIR /app

# Copy go mod and sum files
COPY go.mod go.sum ./

# Download all dependencies. Dependencies will be cached if the go.mod and go.sum files are not changed
RUN go mod download

# Copy the source from the current directory to the Working Directory inside the container
COPY server.go .

# Build the Go apps
RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64  go build -o server server.go

###
### Create image with binary
###

# Start from the latest alpine base image
FROM alpine:latest AS server

# Set the Current Working Directory inside the container
WORKDIR /app

# Copy zoneinfo
RUN apk --no-cache add tzdata

# Copy artifacts created in previous step
COPY --from=builder /app/server .

# Add executable permissions
RUN chmod +x ./server

# Command to run the executable
ENTRYPOINT ["./server"]

# Expose port 80 to the outside world
EXPOSE 80

